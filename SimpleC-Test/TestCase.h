#pragma once

namespace Test
{
    struct TestCase
    {
        int row;
        std::wstring input;
        int output;
    };

    std::vector<TestCase> ReadTestCases(std::wstring_view filepath);
}
