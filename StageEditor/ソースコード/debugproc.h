//===========================================
//
// デバッグ表示[debugproc.h]
// Author:kaiti
//
//===========================================
#ifndef DEBUGPROC_H_
#define DEBUGPROC_H_

#include "main.h"

#define MAX_DEBUGLINE (20)
//デバッグクラス
class CDebugProc
{
public:

	//メンバ関数
	CDebugProc();
	~CDebugProc();

	static void Init(void);
	static void Uninit(void);
	static void Print(const char* fmt, ...);
	static void Draw(void);
private:
	//メンバ変数
	static LPD3DXFONT m_pFont;
	static char m_aStr[MAX_WORD];
	static int m_LineCount;
	static bool m_bUse;
};

#endif