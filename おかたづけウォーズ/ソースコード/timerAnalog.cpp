//===============================================
//
// タイマー[timer.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "timerAnalog.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "debugproc.h"
#include "particle.h"

//===============================================
// コンストラクタ
//===============================================
CAbalogTimer::CAbalogTimer(int nPriority) :CObjectX(nPriority)
{

}
//===============================================
// デストラクタ
//===============================================
CAbalogTimer::~CAbalogTimer()
{
}
//===============================================
// 生成処理
//===============================================
CAbalogTimer* CAbalogTimer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTime)
{
	CAbalogTimer* pTimer = new CAbalogTimer;

	pTimer->Init(pos, rot, nTime);

	return pTimer;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CAbalogTimer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTime)
{
	CObjectX::Init("data\\MODEL\\minuteHand00.x", pos, rot);

	m_pClock = CObjectX::Create("data\\MODEL\\clock00.x", pos, rot);

	// タイマーを設定（秒×60）
	m_nMaxTime = nTime * 60;

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CAbalogTimer::Uninit(void)
{
	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CAbalogTimer::Update(void)
{
	// タイマー加算
	m_nCntTime++;

	if (m_nCntTime >= m_nMaxTime - (m_nMaxTime / 5.0f))
	{
		CntDown();
	}

	// 制限時間経過したら
	if (m_nCntTime >= m_nMaxTime)
	{
		m_nCntTime = 0;	// タイマーをクリア

		// ゲーム終了
		CGame::SetMode(CGame::MODE_FIN);
	}
	// 針を回転
	float fRot = (float)m_nCntTime / (float)m_nMaxTime;
	m_fRotZ = D3DX_PI * 2.0f * fRot;

	// 回転設定
	CObjectX::Set(GetPos(), D3DXVECTOR3(0.0f, 0.0f, -m_fRotZ));

}
//===============================================
// 描画処理
//===============================================
void CAbalogTimer::Draw(void)
{
	CObjectX::Draw();
}
//===============================================
// カウントダウン
//===============================================
void CAbalogTimer::CntDown(void)
{
	D3DXVECTOR3 scale = GetScale();

	// スケール加算
	scale.x += 0.01f;
	scale.y += 0.01f;
	scale.z += 0.01f;

	// スケールが一定以上になったら
	if (scale.x >= 1.5f)
	{
		CParticle::Create(GetPos(), GetRot(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 5, 3.0f, CParticle::TYPE_NONE);	// パーティクル生成
		// スケールを戻す
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	m_pClock->SetScale(scale);
	SetScale(scale);
}
//===============================================
// 時間設定
//===============================================
void CAbalogTimer::SetTime(int nTime)
{
	m_nMaxTime = nTime;
}