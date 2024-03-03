#include "pch.h"
#include "ThrowError.h"
#include "Utils.h"

namespace SimpleC
{
    void ThrowErrorAt(StringView initialInput, StringView input, StringView desc)
    {
        const size_t pos = input.data() - initialInput.data();
        String message{};
        message += L"{}\n"_fmt(initialInput);
        message += L"{:>{}}^ {}\n"_fmt(L" ", pos, desc);

        throw CompileException(message);
    }
}
