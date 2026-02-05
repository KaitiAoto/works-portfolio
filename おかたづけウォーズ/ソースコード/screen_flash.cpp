//===============================================
//
// 画面フラッシュ[screen_flash.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "screen_flash.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//===============================================
// コンストラクタ
//===============================================
CScreenFlash::CScreenFlash(int nPriority):CObject2D(nPriority)
{
	// 値をクリア
	m_bUse = true;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
}
//===============================================
// デストラクタ
//===============================================
CScreenFlash::~CScreenFlash()
{
}
//===============================================
// 生成処理
//===============================================
CScreenFlash* CScreenFlash::Create(const char* pTexName, D3DXCOLOR col, D3DXVECTOR2 size, bool bExpansion)
{
	CScreenFlash* pScreenFlash = new CScreenFlash;

	//初期化
	pScreenFlash->Init(pTexName, col, size, bExpansion);

	return pScreenFlash;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CScreenFlash::Init(const char* pTexName, D3DXCOLOR col, D3DXVECTOR2 size, bool bExpansion)
{
	// 値を代入
	m_bExpansion = bExpansion;	// 拡大するか
	m_col = col;				// 色
	
	// 親の初期化
	CObject2D::Init(pTexName, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), VECTOR3_NULL, D3DXVECTOR2(size.x, size.y));
	
	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CScreenFlash::Uninit(void)
{
	CObject2D::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CScreenFlash::Update(void)
{
	float SubAlpha = 0.01f;	// 減らす量

	// 拡大するなら
	if (m_bExpansion == true)
	{
		SubAlpha = 0.0075f;	// 減らす量を少し小さくする
		// 拡大
		Expansion();
	}

	// 透明度を減らす
	m_col.a -= SubAlpha;

	// 色設定
	CObject2D::SetColor(m_col);

	// 完全に透明になったら
	if (m_col.a <= 0.0f)
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		
		m_bUse = false;

		// 終了
		Uninit();
		return;
	}
}
//===============================================
// 描画処理
//===============================================
void CScreenFlash::Draw(void)
{
	// 使用しているなら
	if (m_bUse == true)
	{
		// ポーズ中じゃないなら
		if (CManager::GetPause() != true)
		{
			// 描画する
			CObject2D::SetUse(true);
			// 描画
			CObject2D::Draw();
		}
		// ポーズ中なら
		else
		{
			// 描画しない
			CObject2D::SetUse(false);
		}
	}
}
//===============================================
// 拡大
//===============================================
void CScreenFlash::Expansion(void)
{
	D3DXVECTOR2 size = GetSize();	// サイズ取得

	// サイズ拡大
	size.x += 25.0f;
	size.y += 25.0f;

	// サイズが画面より大きくなったら
	if (size.x >= SCREEN_WIDTH || size.y >= SCREEN_HEIGHT)
	{
		// 拡大しない
		m_bExpansion = false;
	}

	// サイズ設定
	SetSize(size);
}