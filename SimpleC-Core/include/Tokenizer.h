#pragma once
#include "Forward.h"
#include "Utils.h"

namespace SimpleC
{
    struct TokenReserved
    {
        String str;
    };

    struct TokenNumber
    {
        int value;
    };

    struct TokenEof
    {
    };

    class TokenKind : public EnumValue<int>
    {
    public:
        using EnumValue::EnumValue;
    };

    namespace Tokens
    {
        inline constexpr TokenKind Reserved{0};
        inline constexpr TokenKind Number{1};
        inline constexpr TokenKind Eof{2};
    }

    class TokenType : public std::variant<
            TokenReserved,
            TokenNumber,
            TokenEof>
    {
        using variant::variant;
    };

    struct TokenizedResult
    {
        std::vector<TokenType> tokens;
    };

    TokenizedResult Tokenize(StringView input);
}
