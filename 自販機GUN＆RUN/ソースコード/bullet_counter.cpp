//============================================
//
// 弾数カウンター[bullet_counter.cpp]
// Author : Kaiti Aoto
//
//============================================

// インクルード
#include "bullet_counter.h"
#include "renderer.h"
#include "manager.h"
#include "timer_manager.h"

//==================
// コンストラクタ
//==================
CBullerCounter::CBullerCounter(int nPriority) :CObject(nPriority)
{
	// 値をクリア
	m_pNumber = nullptr;
}
//================
// デストラクタ
//================
CBullerCounter::~CBullerCounter()
{
}
//===========
// 生成処理
//===========
CBullerCounter* CBullerCounter::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 生成
	CBullerCounter* pBullerCounter = new CBullerCounter;
	// 初期化
	if (FAILED(pBullerCounter->Init(pos, fWidth, fHeight)))
	{// NULLチェック
		delete pBullerCounter;
		return nullptr;
	}

	return pBullerCounter;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBullerCounter::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 数字の初期化
	m_pNumber = CNumber::Create("data\\TEXTURE\\number002.png", pos, fWidth, fHeight);

	// オブジェクトの種類設定
	SetObjType(TYPE_BULLETCNT);

	return S_OK;
}
//============
// 終了処理
//============
void CBullerCounter::Uninit(void)
{
	if (m_pNumber != nullptr)
	{// NULLチェック
		// 数字の終了処理＆破棄
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============
// 更新処理
//============
void CBullerCounter::Update(void)
{
}
//============
// 描画処理
//============
void CBullerCounter::Draw(void)
{
	if (m_pNumber != nullptr)
	{// NULLチェック
		// 数字の描画
		m_pNumber->Draw();
	}
}