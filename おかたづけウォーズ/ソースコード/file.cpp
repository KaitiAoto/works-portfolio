//===============================================
//
//  ファイルに関する処理[file.cpp]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "file.h"

//===============================================
// デストラクタ
//===============================================
CFile::~CFile()
{
}
//===============================================
// Int１つのセーブ
//===============================================
void CFile::IntSave(const char* pFileName, const std::vector<int>& data)
{
	// 書き込み用にファイルを開く
	std::ofstream File(pFileName);

	if (!File.is_open())
	{
		std::cerr << "ファイルを開けませんでした！" << std::endl;
		return;
	}

	// 1行に1つ値を書き込む
	for (int value : data)
	{
		File << value << std::endl;
	}

	// ファイルを閉じる
	File.close();
}
//===============================================
// Int１つのロード
//===============================================
std::vector<int> CFile::IntLoad(const char* pFileName)
{
	std::vector<int> data;

	// ファイルを開く
	std::ifstream File(pFileName);

	// ファイルが開けたかチェック
	if (!File.is_open())
	{ // 開けなかった場合
		std::cerr << "ファイルを開けませんでした！" << std::endl;
		return data;
	}

	// ファイルから出力
	int value;
	while (File >> value)
	{
		data.push_back(value);
	}
	// ファイルを閉じる
	File.close();

	return data;
}
