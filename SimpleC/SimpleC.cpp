#include "stdafx.h"

#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

#include "GenerateCode.h"
#include "Parser.h"
#include "Tokenizer.h"

using namespace SimpleC;

namespace
{
}

int main()
{
    if (not setlocale(LC_ALL, "ja_JP.UTF-8"))
    {
        std::wcerr << L"Failed to Set Locale." << std::endl;
    }

    const std::wifstream wif("Scripts/example.txt");
    std::wstringstream wss;
    wss << wif.rdbuf();
    const std::wstring input = wss.str();

    try
    {
        const auto tokens = ExecuteTokenize(input);
        const auto expr = ExecuteParse(tokens);
        const auto code = GenerateCode(*expr);

        std::wcout << code << std::endl;
    }
    catch (const CompileException& exception)
    {
        std::wcerr << exception.message() << std::endl;
    }
}
