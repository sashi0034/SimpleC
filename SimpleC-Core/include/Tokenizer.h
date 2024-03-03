#pragma once
#include <optional>

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

    using TokenType = std::variant<
        TokenReserved,
        TokenNumber,
        TokenEof>;

    struct TokenizedResult
    {
        std::vector<TokenType> tokens;
    };

    TokenizedResult ExecuteTokenize(StringView input);
}
