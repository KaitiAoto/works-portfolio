//===========================================
//
// デバッグ表示[debugproc.h]
// Author : Kaiti Aoto
//
//===========================================

// 二重インクルード防止
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

// インクルード
#include "main.h"

// マクロ定義
#define MAX_DEBUGLINE (20)

// デバッグクラス
class CDebugProc
{
public:

	// メンバ関数
	CDebugProc();
	~CDebugProc();

	// 静的メンバ関数
	static void Init(void);
	static void Uninit(void);
	static void Print(const char* fmt, ...);
	static void Draw(void);
private:
	// 静的メンバ変数
	static LPD3DXFONT m_pFont;		// フォント
	static char m_aStr[MAX_WORD];	// 文字数
	static int m_LineCount;			// 列カウント
	static bool m_bUse;				// 使用しているか
};

#endif