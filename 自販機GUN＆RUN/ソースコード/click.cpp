//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================

#include "click.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CClick::CClick(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = true;
}
//================
// デストラクタ
//================
CClick::~CClick()
{
}
//===========
// 生成処理
//===========
CClick* CClick::Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// メモリ確保
	CClick* pObject2D;
	pObject2D = new CClick;
	//初期化
	pObject2D->Init(pTexName, pos, fWidth, fHeight);
	// ポインタを返す
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CClick::Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//値を代入
	m_pos = pos;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//m_bSub = true;

	// 初期化処理
	CObject2D::Init(pTexName, m_pos, D3DXVECTOR3(0.0f,0.0f,0.0f), fWidth, fHeight);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CClick::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CClick::Update(void)
{
	m_nCntTime++;
	// 点滅させる
	Blink();
}
//============
// 描画処理
//============
void CClick::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 使われていたら
	if (m_bUse == true)
	{
		//ALPHAテストの設定
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		CObject2D::Draw();

		// 設定を元に戻す
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//===========
// 点滅
//===========
void CClick::Blink(void)
{
	// 点滅する速さ
	const float Speed = 0.025f;

	m_col.a = 1.0f * fabsf(sinf(Speed * m_nCntTime));

	// 色を設定
	CObject2D::SetColor(m_col);
}
