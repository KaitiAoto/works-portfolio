//===============================================
//
//  ファイルに関する処理[file.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _FILE_H_
#define _FILE_H_

// イージングクラス
class CFile
{
public:
	// メンバ関数
	~CFile();
	
	// 静的メンバ関数
	static void IntSave(const char* pFileName, const std::vector<int>& data);
	static std::vector<int> IntLoad(const char* pFileName);

private:
	CFile() {};
};

#endif