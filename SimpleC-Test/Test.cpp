#include "stdafx.h"

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "CodeGenerate.h"
#include "Parser.h"
#include "TestCase.h"
#include "Tokenizer.h"

using namespace SimpleC;

namespace
{
    constexpr std::wstring_view outputPath = L"tmp.s";

    constexpr std::string_view targetPath = "../target-linux/";
    constexpr std::string_view batchCommand = "wsl ./assert.sh {}";

    bool runTest(const std::vector<Test::TestCase>& testCases, int i)
    {
        try
        {
            const auto& nextTest = testCases[i];
            auto input = nextTest.input;
            auto expected = nextTest.output;
            const auto tokens = ExecuteTokenize(input);
            const auto expr = ExecuteParse(tokens);
            const auto code = CodeGenerate(*expr);

            std::wofstream outputfile(outputPath.data());
            if (outputfile)
            {
                // std::wcout << code << std::endl;
                outputfile << code << std::endl;
            }
            else
            {
                std::wcout << L"出力ファイルが開けません" << std::endl;
                return false;
            }

            // std::wcout << L">>> {}\t{}\n"_fmt(i, input) << std::flush;
            std::wcout << L"Line={}\t\t=> "_fmt(testCases[i].row) << std::flush;
            const int result = std::system(std::format(batchCommand, expected).data());
        }
        catch (const CompileException& exception)
        {
            std::wcerr << exception.message() << std::endl;
        }
        return true;
    }
}

int main()
{
    constexpr std::wstring_view inputPath = L"Scripts/test.txt";

    if (not setlocale(LC_ALL, "ja_JP.UTF-8"))
    {
        std::wcerr << L"Failed to Set Locale." << std::endl;
    }

    const auto testCases = Test::ReadTestCases(inputPath);

    current_path(std::filesystem::current_path() / targetPath);

    for (int i = 0; i < testCases.size(); ++i)
    {
        if (not runTest(testCases, i)) return 1;
    }
}
