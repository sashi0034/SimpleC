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

    struct TokenBase
    {
        explicit TokenBase(int pos): pos(pos) { return; }
        virtual ~TokenBase() = default;
        virtual TokenKind Kind() const = 0;

        int pos;
    };

    struct TokenReserved : TokenBase
    {
        TokenReserved(int pos, String str);
        TokenKind Kind() const override { return TokenKind::Reserved; }

        String str;
    };

    struct TokenIdent : TokenBase
    {
        explicit TokenIdent(int pos, String str);
        TokenKind Kind() const override { return TokenKind::Ident; }

        String str;
    };

    struct TokenNumber : TokenBase
    {
        TokenNumber(int pos, int v);
        TokenKind Kind() const override { return TokenKind::Reserved; }

        int value;
    };

    struct TokenEof : TokenBase
    {
        using TokenBase::TokenBase;
        TokenKind Kind() const override { return TokenKind::Eof; }
    };

    using TokenPtr = std::shared_ptr<TokenBase>;

    struct TokenizedResult
    {
        StringView input;
        std::vector<int> tokenPositions;
        std::vector<TokenPtr> tokens;
    };

    TokenizedResult ExecuteTokenize(StringView input);
}
