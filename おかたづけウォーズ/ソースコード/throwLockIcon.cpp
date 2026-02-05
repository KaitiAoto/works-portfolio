//===============================================
//
// 投げられない時用アイコン[throwLockIcon.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "throwLockIcon.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

//===============================================
// コンストラクタ
//===============================================
CThrowLockIcon::CThrowLockIcon(int nPriority):CObjectBillboard(nPriority)
{
	// 値をクリア
	m_bUse = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}
//===============================================
// デストラクタ
//===============================================
CThrowLockIcon::~CThrowLockIcon()
{
}
//===============================================
// 生成処理
//===============================================
CThrowLockIcon* CThrowLockIcon::Create(const char* pTexName, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size)
{
	// インスタンス生成
	CThrowLockIcon* pIcon = new CThrowLockIcon;

	//初期化
	if (FAILED(pIcon->Init(pTexName, pos, col, size)))
	{
		delete pIcon;
		return nullptr;
	}

	return pIcon;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CThrowLockIcon::Init(const char* pTexName, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size)
{
	// 値を代入
	m_col = col;	// 色
	
	// 親の初期化
	CObjectBillboard::Init(pTexName, pos, VECTOR3_NULL, size.x, size.y);
	
	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CThrowLockIcon::Uninit(void)
{
	// 親クラスの終了処理
	CObjectBillboard::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CThrowLockIcon::Update(void)
{
	// 使用しているなら
	if (m_bUse == true)
	{
		float SubAlpha = 0.005f;	// 減らす量

		// 透明度を減らす
		m_col.a -= SubAlpha;

		// 色設定
		CObjectBillboard::SetColor(m_col);

		// 完全に透明になったら
		if (m_col.a <= 0.0f)
		{
			// 色を戻す
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_bUse = false;

			return;
		}
	}
}
//===============================================
// 描画処理
//===============================================
void CThrowLockIcon::Draw(void)
{
	// 使用しているなら
	if (m_bUse == true)
	{
		// ポーズ中じゃないなら
		if (CManager::GetPause() != true)
		{
			// 描画する
			CObjectBillboard::SetDraw(true);
			// 描画
			CObjectBillboard::Draw();
		}
		// ポーズ中なら
		else
		{
			// 描画しない
			CObjectBillboard::SetDraw(false);
		}
	}
}