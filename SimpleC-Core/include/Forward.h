#pragma once
#include "../pch.h"

namespace SimpleC
{
    using String = std::u32string;
    using StringView = std::u32string_view;

    class CompileException : public std::exception
    {
        using std::exception::exception;
    };
}
