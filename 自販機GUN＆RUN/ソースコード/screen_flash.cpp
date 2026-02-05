//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "screen_flash.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CScreenFlash::CScreenFlash(int nPriority):CObject2D(nPriority)
{
	m_bUse = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
}
//================
// デストラクタ
//================
CScreenFlash::~CScreenFlash()
{
}
//===========
// 生成処理
//===========
CScreenFlash* CScreenFlash::Create(const char* pTexName, D3DXCOLOR col)
{
	CScreenFlash* pObject2D;
	pObject2D = new CScreenFlash;
	//初期化
	pObject2D->Init(pTexName, col);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CScreenFlash::Init(const char* pTexName, D3DXCOLOR col)
{
	m_col = col;	
	if (m_col.a <= 0.0f)
	{
		m_bUse = false;
	}
	else
	{
		m_bUse = true;
	}
	CObject2D::Init(pTexName, { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { 0.0f,0.0f,0.0f }, SCREEN_WIDTH, SCREEN_HEIGHT);
	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CScreenFlash::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CScreenFlash::Update(void)
{
	const float SubAlpha = 0.01f;

	m_col.a -= SubAlpha;

	if (m_col.a <= 0.0f)
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		m_bUse = false;
	}

	CObject2D::SetColor(m_col);
}
//============
// 描画処理
//============
void CScreenFlash::Draw(void)
{
	if (m_bUse == true)
	{
		if (CManager::GetPause() != true)
		{
			CObject2D::SetUse(true);
			CObject2D::Draw();
		}
		else
		{
			CObject2D::SetUse(false);
		}
	}
}