#include "pch.h"
#include "Parser.h"

#include "ThrowError.h"
#include "Utils.h"

using namespace SimpleC;
using std::unique_ptr;

namespace
{
    struct LVar
    {
        StringView name;
        int offset;
    };

    struct ReadingTokens
    {
        StringView input;
        std::span<TokenPtr> tokens;
        int head;
        std::vector<LVar> lvars;

        const TokenBase& Next() const
        {
            return *tokens[head];
        }

        bool IsEnd() const { return head == tokens.size(); }
    };

    int searchLvarOffset(ReadingTokens& reading, StringView ident)
    {
        const auto found = std::ranges::find_if(reading.lvars, [&](const LVar& lv)
        {
            return lv.name == ident;
        });
        if (found != reading.lvars.end())
        {
            // 既に登場したローカル変数
            return found->offset;
        }

        // 新規ローカル変数
        const auto newLvar = LVar{.name = ident, .offset = static_cast<int>(reading.lvars.size() + 1) * 8};
        reading.lvars.push_back(newLvar);
        return newLvar.offset;
    }

    bool tryConsume(ReadingTokens& reading, StringView op)
    {
        if (reading.IsEnd()) return false;
        if (const auto t = dynamic_cast<const TokenReserved*>(&reading.Next());
            t != nullptr && t->str == op)
        {
            reading.head++;
            return true;
        }
        return false;
    }

    const TokenIdent* tryConsumeIdent(ReadingTokens& reading)
    {
        if (const auto t = dynamic_cast<const TokenIdent*>(&reading.Next()))
        {
            reading.head++;
            return t;
        }
        return nullptr;
    }

    void expect(ReadingTokens& reading, StringView op)
    {
        if (reading.IsEnd())
        {
        }
        else if (const auto t = dynamic_cast<const TokenReserved*>(&reading.Next());
            t && t->str == op)
        {
            // OK
            reading.head++;
            return;
        }

        ThrowErrorAt(reading.input, reading.Next().pos, L"{}ではありません"_fmt(op));
    }

    int expectNumber(ReadingTokens& reading)
    {
        if (const auto t = dynamic_cast<const TokenNumber*>(&reading.Next()))
        {
            reading.head++;
            return t->value;
        }
        ThrowErrorAt(reading.input, reading.Next().pos, L"数ではありません");
    }

    unique_ptr<NodeObject> expr(ReadingTokens& reading);

    // primary    = num | ident | "(" expr ")"
    unique_ptr<NodeObject> primary(ReadingTokens& reading)
    {
        if (const auto ident = tryConsumeIdent(reading))
        {
            // 左辺値
            const int offset = searchLvarOffset(reading, ident->str);
            return std::make_unique<NodeLvar>(offset);
        }

        // 次のトークンが"("なら、"(" expr ")"のはず
        if (tryConsume(reading, L"("))
        {
            auto node = expr(reading);
            expect(reading, L")");
            return node;
        }

        // そうでなければ数値のはず
        NodeNumber nodeNumber{};
        nodeNumber.value = expectNumber(reading);
        return std::make_unique<NodeNumber>(nodeNumber);
    }

    // unary      = ("+" | "-")? primary
    unique_ptr<NodeObject> unary(ReadingTokens& reading)
    {
        if (tryConsume(reading, L"+"))
            return primary(reading);
        if (tryConsume(reading, L"-"))
            return std::make_unique<NodeSub>(std::make_unique<NodeNumber>(0), primary(reading));
        return primary(reading);
    }

    // mul        = unary ("*" unary | "/" unary)*
    unique_ptr<NodeObject> mul(ReadingTokens& reading)
    {
        unique_ptr<NodeObject> node = unary(reading);

        while (true)
        {
            if (tryConsume(reading, L"*"))
            {
                auto newNode = NodeMul(std::move(node), unary(reading));
                node = std::make_unique<NodeMul>(std::move(newNode));
            }
            else if (tryConsume(reading, L"/"))
            {
                auto newNode = NodeDiv(std::move(node), unary(reading));
                node = std::make_unique<NodeDiv>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }

    // dd        = mul ("+" mul | "-" mul)*
    unique_ptr<NodeObject> add(ReadingTokens& reading)
    {
        unique_ptr<NodeObject> node = mul(reading);

        while (true)
        {
            if (tryConsume(reading, L"+"))
            {
                auto newNode = NodeAdd(std::move(node), mul(reading));
                node = std::make_unique<NodeAdd>(std::move(newNode));
            }
            else if (tryConsume(reading, L"-"))
            {
                auto newNode = NodeSub(std::move(node), mul(reading));
                node = std::make_unique<NodeSub>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }

    // relational = add ("<" add | "<=" add | ">" add | ">=" add)*
    unique_ptr<NodeObject> relational(ReadingTokens& reading)
    {
        unique_ptr<NodeObject> node = add(reading);

        while (true)
        {
            if (tryConsume(reading, L"<"))
            {
                auto newNode = NodeLt(std::move(node), add(reading));
                node = std::make_unique<NodeLt>(std::move(newNode));
            }
            else if (tryConsume(reading, L"<="))
            {
                auto newNode = NodeLe(std::move(node), add(reading));
                node = std::make_unique<NodeLe>(std::move(newNode));
            }
            else if (tryConsume(reading, L">"))
            {
                auto newNode = NodeLt(add(reading), std::move(node));
                node = std::make_unique<NodeLt>(std::move(newNode));
            }
            else if (tryConsume(reading, L">="))
            {
                auto newNode = NodeLe(add(reading), std::move(node));
                node = std::make_unique<NodeLe>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }

    // equality   = relational ("==" relational | "!=" relational)*
    unique_ptr<NodeObject> equality(ReadingTokens& reading)
    {
        unique_ptr<NodeObject> node = relational(reading);

        while (true)
        {
            if (tryConsume(reading, L"=="))
            {
                auto newNode = NodeEq(std::move(node), relational(reading));
                node = std::make_unique<NodeEq>(std::move(newNode));
            }
            else if (tryConsume(reading, L"!="))
            {
                auto newNode = NodeNe(std::move(node), relational(reading));
                node = std::make_unique<NodeNe>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }

    // assign     = equality ("=" assign)?
    NodePtr assign(ReadingTokens& reading)
    {
        NodePtr node = equality(reading);
        if (tryConsume(reading, L"="))
        {
            node = std::make_unique<NodeAssign>(std::move(node), assign(reading));
        }
        return node;
    }

    // expr       = assign
    NodePtr expr(ReadingTokens& reading)
    {
        return assign(reading);
    }

    // stmt       = expr ";"
    NodePtr stmt(ReadingTokens& reading)
    {
        NodePtr node = expr(reading);
        expect(reading, L";");
        return node;
    }

    // program    = stmt*
    ProgramNodes program(ReadingTokens& reading)
    {
        ProgramNodes result{};

        while (reading.Next().Kind() != TokenKind::Eof)
        {
            result.push_back(stmt(reading));
        }

        return result;
    }
}

namespace SimpleC
{
    NodeBranch::NodeBranch(unique_ptr<NodeObject> lhs, unique_ptr<NodeObject> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    {
    }

    ProgramNodes ExecuteParse(const TokenizedResult& input)
    {
        auto tokens = input.tokens;
        auto reading = ReadingTokens{
            .input = input.input,
            .tokens = tokens,
            .head = 0,
        };
        return program(reading);
    }
}
