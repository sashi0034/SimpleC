﻿#include "stdafx.h"

#include <format>

#include "../SimpleC-Core/include/Example.h"

int main()
{
    std::cout << std::format("Example is {}\n", SimpleC::ExampleValue(1));

    std::ifstream file("Scripts/example.txt");

    if (file.is_open())
    {
        std::cout << "Opened file!" << std::endl;
        std::string line;
        while (getline(file, line))
        {
            std::cout << line << '\n';
        }
        file.close();
    }
    else
    {
        std::cout << "Failed to open file" << std::endl;
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
