#include "stdafx.h"

#include <codecvt>
#include <filesystem>
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
    constexpr std::wstring_view inputPath = L"Scripts/example.txt";

    constexpr std::string_view targetPath = "../target-linux/";
    constexpr std::wstring_view outputPath = L"tmp.s";
    constexpr std::string_view batchPath = "wsl ./run.sh";

    if (not setlocale(LC_ALL, "ja_JP.UTF-8"))
    {
        std::wcerr << L"Failed to Set Locale." << std::endl;
    }

    const std::wifstream wif(inputPath.data());
    std::wstringstream wss;
    wss << wif.rdbuf();
    const std::wstring input = wss.str();

    try
    {
        const auto tokens = ExecuteTokenize(input);
        const auto expr = ExecuteParse(tokens);
        const auto code = GenerateCode(*expr);

        current_path(std::filesystem::current_path() / targetPath);
        std::wofstream outputfile(outputPath.data());
        if (outputfile)
        {
            std::wcout << code << std::endl;
            outputfile << code << std::endl;
        }
        else
        {
            std::wcout << L"出力ファイルが開けません" << std::endl;
            return 1;
        }

        std::wcout << L"結果\n{} => "_fmt(input) << std::flush;
        const int result = std::system(batchPath.data());
    }
    catch (const CompileException& exception)
    {
        std::wcerr << exception.message() << std::endl;
    }
}
