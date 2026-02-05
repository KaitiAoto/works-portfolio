//=======================
//
// タイマー[timer.cpp]
// Author : Kaiti Aoto
//
//=======================
#include "timer.h"
#include "renderer.h"
#include "manager.h"
#include "timer_manager.h"

//==================
// コンストラクタ
//==================
CTimer::CTimer(int nPriority) :CObject(nPriority)
{
	m_pNumber = nullptr;
}
//================
// デストラクタ
//================
CTimer::~CTimer()
{
}
//===========
// 生成処理
//===========
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer* pScore = new CTimer;
	//初期化
	pScore->Init(pos, TIMER_SIZE, TIMER_SIZE);

	return pScore;
}
//===============
// 初期化処理
//===============
HRESULT CTimer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	//初期化
	m_pNumber = CNumber::Create("data\\TEXTURE\\number002.png", pos, fWidth, fHeight);
	
	SetObjType(TYPE_TIME);

	return S_OK;
}
//============
// 終了処理
//============
void CTimer::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = nullptr;
	}
	CObject::Release();
}
//============
// 更新処理
//============
void CTimer::Update(void)
{
}
//============
// 描画処理
//============
void CTimer::Draw(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Draw();
	}
}