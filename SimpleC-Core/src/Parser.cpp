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

    unique_ptr<NodeType> expr(ReadingTokens& reading);

    // primary = num | "(" expr ")"
    unique_ptr<NodeType> primary(ReadingTokens& reading)
    {
        // 次のトークンが"("なら、"(" expr ")"のはず
        if (tryConsume(reading, L"("))
        {
            auto node = expr(reading);
            expect(reading, L")");
            return node;
        }

        // そうでなければ数値のはず
        return std::make_unique<NodeType>(NodeNumber{.value = expectNumber(reading)});
    }

    // mul     = primary ("*" primary | "/" primary)*
    unique_ptr<NodeType> mul(ReadingTokens& reading)
    {
        unique_ptr<NodeType> node = primary(reading);

        while (true)
        {
            if (tryConsume(reading, L"*"))
            {
                auto newNode = NodeMul(std::move(node), primary(reading));
                node = std::make_unique<NodeType>(std::move(newNode));
            }
            else if (tryConsume(reading, L"/"))
            {
                auto newNode = NodeDiv(std::move(node), primary(reading));
                node = std::make_unique<NodeType>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }

    // expr    = mul ("+" mul | "-" mul)*
    unique_ptr<NodeType> expr(ReadingTokens& reading)
    {
        unique_ptr<NodeType> node = mul(reading);

        while (true)
        {
            if (tryConsume(reading, L"+"))
            {
                auto newNode = NodeAdd(std::move(node), mul(reading));
                node = std::make_unique<NodeType>(std::move(newNode));
            }
            else if (tryConsume(reading, L"-"))
            {
                auto newNode = NodeSub(std::move(node), mul(reading));
                node = std::make_unique<NodeType>(std::move(newNode));
            }
            else
            {
                return node;
            }
        }
    }
}

namespace SimpleC
{
    NodeBranch::NodeBranch(unique_ptr<NodeType> lhs, unique_ptr<NodeType> rhs) :
        lhs(std::move(lhs)),
        rhs(std::move(rhs))
    {
    }

    NodeType ExecuteParse(const TokenizedResult& input)
    {
        auto tokens = input.tokens;
        auto reading = ReadingTokens{
            .tokens = tokens,
            .head = 0,
        };
        const auto result = expr(reading);
        return std::move(*result);
    }
}
