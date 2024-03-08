#include "pch.h"
#include "ThrowError.h"
#include "Utils.h"

using namespace SimpleC;

namespace
{
    std::vector<String> splitString(StringView str)
    {
        std::vector<String> tokens;
        std::wstringstream ss(str.data());
        std::wstring token;
        while (std::getline(ss, token, L'\n'))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
}

namespace SimpleC
{
    void ThrowErrorAt(StringView initialInput, StringView input, StringView desc)
    {
        const size_t pos = input.data() - initialInput.data();
        ThrowErrorAt(initialInput, pos, desc);
    }

    void ThrowErrorAt(StringView initialInput, int pos, StringView desc)
    {
        const auto lines = splitString(initialInput);

        String message{};
        for (int i = 0; i < lines.size(); ++i)
        {
            const auto lineSize = lines[i].size() + 1; // 本来の改行込みのサイズ
            if (lineSize >= pos)
            {
                message += L"> ({}, {})\n"_fmt(i + 1, pos + 1);
                message += L"{}\n"_fmt(lines[i]);
                message += L"{:>{}}^ {}\n"_fmt(L" ", pos, desc);
                break;
            }
            pos -= lineSize;
        }

        throw CompileException(message);
    }
}
