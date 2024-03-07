#pragma once

#include "Forward.h"

namespace SimpleC
{
    enum class TokenKind
    {
        Reserved,
        Ident,
        Number,
        Eof,
    };

    class TokenBase
    {
    public:
        virtual ~TokenBase() = default;
        virtual TokenKind Kind() const = 0;
    };

    struct TokenReserved : TokenBase
    {
        TokenReserved(String str = {}) : str(std::move(str)) { return; }
        TokenKind Kind() const override { return TokenKind::Reserved; }

        String str;
    };

    struct TokenIdent : TokenBase
    {
        explicit TokenIdent(String str) : str(std::move(str)) { return; }
        TokenKind Kind() const override { return TokenKind::Ident; }

        String str;
    };

    struct TokenNumber : TokenBase
    {
        TokenNumber(int v = {}) : value(v) { return; }
        TokenKind Kind() const override { return TokenKind::Reserved; }

        int value;
    };

    struct TokenEof : TokenBase
    {
        TokenKind Kind() const override { return TokenKind::Eof; }
    };

    using TokenPtr = std::shared_ptr<TokenBase>;

    struct TokenizedResult
    {
        std::vector<TokenPtr> tokens;
    };

    TokenizedResult ExecuteTokenize(StringView input);
}
