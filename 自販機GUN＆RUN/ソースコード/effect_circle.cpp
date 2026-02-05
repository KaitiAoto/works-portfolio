//==============================
//
// パーティクル[particle.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "effect_circle.h"
#include "renderer.h"
#include "manager.h"

//==================
// コンストラクタ
//==================
CEffectCircle::CEffectCircle(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_fRadius = 0;
	m_nLife = 0;
	m_bUse = false;
	m_bUp = true;
}
//================
// デストラクタ
//================
CEffectCircle::~CEffectCircle()
{
}
//===========
// 生成処理
//===========
CEffectCircle* CEffectCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, CMeshCircle::TYPE circle, TYPE type)
{
	CEffectCircle* pParticle;
	pParticle = new CEffectCircle;
	//初期化
	pParticle->Init(pos, rot, fRadius, col,  circle, type);
	return pParticle;
}
//===============
// 初期化処理
//===============
HRESULT CEffectCircle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, CMeshCircle::TYPE circle, TYPE type)
{
	//値を代入
	m_pos = pos;
	m_posOff = pos;
	m_rot = rot;
	m_bUse = true;
	m_type = type;
	m_col = col;

	m_pCircle = CMeshCircle::Create(m_pos, m_rot, fRadius, col, circle);


	return S_OK;
}
//============
// 終了処理
//============
void CEffectCircle::Uninit(void)
{
	if (m_pCircle != nullptr)
	{
		m_pCircle->Uninit();
	}

	CObject::Release();
}
//============
// 更新処理
//============
void CEffectCircle::Update(void)
{
	if (m_bUse == true)
	{
		switch (m_type)
		{
		case TYPE_UPDOWN:
			UpDown();
			break;
		default:
			break;
		}
	}
	else if (m_bUse == false)
	{
		Uninit();
		return;
	}
	//m_nLife --;
	//if (m_nLife <= 0)
	//{
	//	m_bUse = false;
	//}
}
//============
// 描画処理
//============
void CEffectCircle::Draw(void)
{
}
void CEffectCircle::UpDown(void)
{
	if (m_bUp == true)
	{
		m_pos.y += 1.0f;

		if (m_pos.y >= m_posOff.y + 30.0f)
		{
			m_bUp = false;
		}
	}
	else
	{
		m_pos.y -= 1.0f;

		if (m_pos.y <= m_posOff.y)
		{
			m_bUse = false;
		}
	}



	m_pCircle->SetPos(m_pos);
}