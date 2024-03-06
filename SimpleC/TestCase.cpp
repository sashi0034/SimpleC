#include "stdafx.h"
#include "TestCase.h"

namespace
{
    enum class NextRead
    {
        Idle,
        Input,
        Output,
    };
}

namespace Test
{
    bool tryReadNumber(std::wstring line, int& number)
    {
        const std::string narrowLine(line.begin(), line.end());
        try
        {
            number = std::stoi(narrowLine);
            return true;
        }
        catch (const std::invalid_argument&)
        {
            std::wcerr << L"無効な数値: " << narrowLine.c_str() << std::endl;
        } catch (const std::out_of_range&)
        {
            std::wcerr << L"数値が範囲外です: " << narrowLine.c_str() << std::endl;
        }
        return false;
    }

    std::vector<TestCase> ReadTestCases(std::wstring_view filepath)
    {
        std::wifstream file(filepath.data());
        if (not file)
        {
            std::wcerr << L"ファイルが開けません: " << filepath << std::endl;
            return {};
        }

        std::vector<TestCase> result{};
        TestCase nextTest{};
        NextRead nextRead = NextRead::Idle;
        std::wstring line;
        int row{1};
        while (std::getline(file, line))
        {
            row++;
            const bool isNullOrWhitespace = line.empty() || std::ranges::all_of(line, [](wchar_t c)
            {
                return std::isspace(c);
            });
            if (isNullOrWhitespace) continue;

            // 状態切替
            if (line == L"[in]")
            {
                if (nextRead != NextRead::Idle)
                {
                    std::wcerr << L"構文に誤りがあります" << filepath << std::endl;
                    return {};
                }
                nextTest = {};
                nextRead = NextRead::Input;
                continue;
            }
            if (line == L"[out]")
            {
                if (nextRead != NextRead::Input)
                {
                    std::wcerr << L"構文に誤りがあります" << filepath << std::endl;
                    return {};
                }
                nextRead = NextRead::Output;
                continue;
            }

            // 読み取り
            if (nextRead == NextRead::Input)
            {
                // 入力読み込み
                if (not nextTest.input.empty()) nextTest.input += L"\n";
                else nextTest.row = row;
                nextTest.input += line;
            }
            else if (nextRead == NextRead::Output)
            {
                // 出力読み込み
                if (tryReadNumber(line, nextTest.output))
                {
                    result.push_back(nextTest);
                    nextRead = NextRead::Idle;
                }
                else
                {
                    return {};
                }
            }
        }
        return result;
    }
}
