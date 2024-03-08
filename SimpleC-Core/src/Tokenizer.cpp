﻿#include "pch.h"
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

    bool isAlnum(wchar_t c)
    {
        return ('a' <= c && c <= 'z')
            || ('A' <= c && c <= 'Z')
            || ('0' <= c && c <= '9')
            || (c == '_');
    }

    constexpr std::array allSymbols{
        // 文字数が多い順から判定をする
        L"<=", L">=", L"==", L"!=",
        L"+", L"-", L"*", L"/", L"(", L")", L"<", L">", L"=", L";"
    };

    StringView trySymbol(StringView input)
    {
        for (const auto& s : allSymbols)
        {
            if (input.starts_with(s)) return s;
        }
        return {};
    }

    StringView tryIdent(StringView input)
    {
        for (int i = 0; ; ++i)
        {
            const bool isIdentChar = (i < input.size() - 1 && isAlnum(input[i]));

            if (not isIdentChar)
            {
                return i > 0 ? StringView(input.data(), i) : StringView();
            }
        }
    }

    bool isReservedWord(const StringView ident)
    {
        return ident == L"return";
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
            if (std::isspace(front) || front == L'\r' || front == L'\n')
            {
                input.remove_prefix(1);
                continue;
            }

            // 記号
            if (const auto symbol = trySymbol(input); not symbol.empty())
            {
                input.remove_prefix(symbol.size());
                result.tokens.emplace_back(std::make_shared<TokenReserved>(symbol.data()));
                continue;
            }

            // 数値
            if (std::isdigit(front))
            {
                result.tokens.emplace_back(std::make_shared<TokenNumber>(takeTokenNumber(input)));
                continue;
            }

            // 識別子
            if (const auto ident = tryIdent(input); not ident.empty())
            {
                input.remove_prefix(ident.size());
                if (isReservedWord(ident)) result.tokens.emplace_back(std::make_shared<TokenReserved>(String(ident)));
                else result.tokens.emplace_back(std::make_shared<TokenIdent>(String(ident)));
                continue;
            }

            ThrowErrorAt(initialInput, input, L"数値ではありません");
        }

        result.tokens.emplace_back(std::make_shared<TokenEof>());
        return result;
    }
}
