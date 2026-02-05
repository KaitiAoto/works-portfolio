//==============================
//
//  タイマー処理[.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge_enemy.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
#include "object2D.h"
//==================
// コンストラクタ
//==================
CEnemyGauge::CEnemyGauge(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
	m_pGauge = nullptr;
}
//================
// デストラクタ
//================
CEnemyGauge::~CEnemyGauge()
{
}
//===========
// 生成処理
//===========
CEnemyGauge* CEnemyGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CEnemyGauge* pGause;
	//
	pGause = new CEnemyGauge;
	//初期化
	if (FAILED(pGause->Init(pos, base, fHeight, col)))
	{
		delete pGause;
		return nullptr;
	}

	return pGause;
}
//=============
// 初期化処理
//=============
HRESULT CEnemyGauge::Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	m_pos = pos;
	m_Base = base;
	m_fHeight = fHeight;
	m_col = col;

	m_pGauge = CGaugeBillboard::Create(m_pos, m_Base, m_fHeight, m_col);
	//CObject2D::Create("data\\TEXTURE\\gaugeHP01.png", D3DXVECTOR3(m_pos.x + (m_Base * GAUGE_X / 2.2f) + 10.0f, m_pos.y, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (float)(m_Base * GAUGE_X) * 1.17f, (m_fHeight * 2) + 50.0f);

	//オブジェクトの種類設定
	SetObjType(TYPE_GAUGE);

	return S_OK;
}
//==========
// 終了処理
//==========
void CEnemyGauge::Uninit(void)
{
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		delete m_pGauge;
		m_pGauge = nullptr;
	}
	CObject::Release();
}
//==========
// 更新処理
//==========
void CEnemyGauge::Update(void)
{
	if (m_rate > 0.7f)
	{
		// 明るい水色
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f,1.0f);
	}
	else if (m_rate > 0.4f)
	{
		// オレンジ
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	}
	else if (m_rate > 0.1f)
	{
		// ピンク寄りの赤
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (m_bDraw == true)
	{
		m_pGauge->SetBase(m_Base);
		m_pGauge->SetRate(m_rate);
		m_pGauge->SetColor(m_col);
		m_pGauge->SetPos(m_pos);
		m_pGauge->Set();
	}
}
//===========
// 描画処理
//===========
void CEnemyGauge::Draw(void)
{
	if (m_bDraw == true)
	{
		m_pGauge->Draw();
	}
}