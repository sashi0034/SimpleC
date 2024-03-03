#include "stdafx.h"

#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>

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

        std::cout << expr.index() << std::endl;
    }
    catch (const CompileException& exception)
    {
        std::wcerr << exception.message() << std::endl;
    }
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
