#pragma once
#include "../pch.h"

namespace SimpleC
{
    using String = std::wstring;
    using StringView = std::wstring_view;

    class CompileException : public std::exception
    {
    public:
        CompileException(StringView what) :
            std::exception("CompileException"),
            m_message(what)
        {
        }

        const String& message() const { return m_message; }

    private:
        String m_message;
    };
}
