#include "pch.h"
#include "Tokenizer.h"

#include "ThrowError.h"

using namespace SimpleC;

namespace
{
    TokenNumber takeTokenNumber(StringView& input)
    {
        TokenNumber token{};
        while (true)
        {
            if (input.empty()) break;
            const auto front = input.front();
            if (U'0' <= front && front <= U'9')
            {
                token.value = token.value * 10 + (front - U'0');
                input.remove_prefix(1);
            }
            else
            {
                break;
            }
        }
        return token;
    }

    constexpr std::array allSymbols{L"+", L"-", L"*", L"/", L"(", L")"};

    bool isSymbol(StringView input)
    {
        for (const auto s : allSymbols)
        {
            if (input[0] == s[0]) return true;
        }
        return false;
    }
}

namespace SimpleC
{
    TokenizedResult ExecuteTokenize(StringView input)
    {
        const StringView initialInput = input;
        TokenizedResult result{};

        while (not input.empty())
        {
            const char32_t front = input.front();

            // 空白文字をスキップ
            if (std::isspace(front))
            {
                input.remove_prefix(1);
                continue;
            }

            // 記号
            if (isSymbol(input))
            {
                String str{};
                str.push_back(front);
                input.remove_prefix(1);
                result.tokens.emplace_back(TokenReserved{.str = str});
                continue;
            }

            // 数値
            if (std::isdigit(front))
            {
                result.tokens.emplace_back(takeTokenNumber(input));
                continue;
            }

            ThrowErrorAt(initialInput, input, L"数値ではありません");
        }

        result.tokens.emplace_back(TokenEof());
        return result;
    }
}
