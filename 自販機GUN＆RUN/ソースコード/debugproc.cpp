//===========================================
//
// デバッグ表示[debugproc.cpp]
// Author : Kaiti Aoto
//
//===========================================
#include "debugproc.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
//静的メンバ変数
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[MAX_WORD] = {};
int CDebugProc::m_LineCount = 0;
bool CDebugProc::m_bUse = true;
//==================
// コンストラクタ
//==================
CDebugProc::CDebugProc()
{

}
//================
// デストラクタ
//================
CDebugProc::~CDebugProc()
{

}
//===============
// 初期化処理
//===============
void CDebugProc::Init(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	memset(m_aStr, 0, MAX_WORD);
}
//============
// 終了処理
//============
void CDebugProc::Uninit(void)
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}
//================
// 情報書き込み
//================
void CDebugProc::Print(const char* fmt, ...)
{
	char buf[512];

	va_list ap;

	va_start(ap, fmt);

	vsnprintf(buf, sizeof(buf), fmt, ap);

	va_end(ap);

	// バッファに追記
	size_t currentLen = strlen(m_aStr);
	size_t bufLen = strlen(buf);

	if (currentLen + bufLen + 2 < MAX_WORD)
	{
		strcat(m_aStr, buf);
		strcat(m_aStr, "\n");
	}
}
//============
// 描画処理
//============
void CDebugProc::Draw(void)
{
#ifdef _DEBUG
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	if (pInputKey->GetTrigger(DIK_F1) == true)
	{//bUse切り替え
		m_bUse = !m_bUse;
	}
	if (m_bUse)
	{
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		memset(m_aStr, 0, MAX_WORD);
	}
#endif
}