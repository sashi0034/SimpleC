#include "pch.h"
#include "Parser.h"
#include "Utils.h"

using namespace SimpleC;
using std::unique_ptr;

namespace
{
    struct ReadingTokens
    {
        std::span<TokenType> tokens;
        int head;

        const TokenType& Next() const
        {
            return tokens[head];
        }

        bool IsEnd() const { return head == tokens.size(); }
    };

    bool tryConsume(ReadingTokens& reading, StringView op)
    {
        if (reading.IsEnd()) return false;
        if (const auto t = std::get_if<TokenReserved>(&reading.Next());
            t != nullptr && t->str == op)
        {
            reading.head++;
            return true;
        }
        return false;
    }

    void expect(ReadingTokens& reading, StringView op)
    {
        if (reading.IsEnd())
        {
        }
        else if (const auto t = std::get_if<TokenReserved>(&reading.Next());
            t && t->str == op)
        {
            // OK
            reading.head++;
            return;
        }

        throw CompileException(L"{}ではありません"_fmt(op));
    }

    int expectNumber(ReadingTokens& reading)
    {
        if (const auto t = std::get_if<TokenNumber>(&reading.Next()))
        {
            reading.head++;
            return t->value;
        }
        throw CompileException(L"数ではありません");
    }

    unique_ptr<NodeObject> expr(ReadingTokens& reading);

    // primary = num | "(" expr ")"
    unique_ptr<NodeObject> primary(ReadingTokens& reading)
    {
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

    // unary   = ("+" | "-")? primary
    unique_ptr<NodeObject> unary(ReadingTokens& reading)
    {
        if (tryConsume(reading, L"+"))
            return primary(reading);
        if (tryConsume(reading, L"-"))
            return std::make_unique<NodeSub>(std::make_unique<NodeNumber>(0), primary(reading));
        return primary(reading);
    }

    // mul     = unary ("*" unary | "/" unary)*
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

    // add = mul ("+" mul | "-" mul)*
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

    // equality = relational ("==" relational | "!=" relational)*
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

    // expr = equality
    unique_ptr<NodeObject> expr(ReadingTokens& reading)
    {
        return equality(reading);
    }
}

namespace SimpleC
{
    NodeBranch::NodeBranch(unique_ptr<NodeObject> lhs, unique_ptr<NodeObject> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    {
    }

    std::unique_ptr<NodeObject> ExecuteParse(const TokenizedResult& input)
    {
        auto tokens = input.tokens;
        auto reading = ReadingTokens{
            .tokens = tokens,
            .head = 0,
        };
        return expr(reading);
    }
}
