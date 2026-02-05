//==============================
//
// パーティクル[particle.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "particle.h"
#include "renderer.h"
#include "manager.h"

//==================
// コンストラクタ
//==================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_fRadius = 0;
	m_nLife = 0;
	m_bUse = false;
}
//================
// デストラクタ
//================
CParticle::~CParticle()
{
}
//===========
// 生成処理
//===========
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fRadius, TYPE type)
{
	CParticle* pParticle;
	pParticle = new CParticle;
	//初期化
	pParticle->Init(pos, rot, type);
	pParticle->SetElse(col, nLife, fRadius);
	return pParticle;
}
//===============
// 初期化処理
//===============
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	//値を代入
	m_pos = pos;
	m_rot = rot;
	m_bUse = true;
	m_type = type;

	return S_OK;
}
//============
// 終了処理
//============
void CParticle::Uninit(void)
{
	CObject::Release();
}
//============
// 更新処理
//============
void CParticle::Update(void)
{
	if (m_bUse == true)
	{
		switch (m_type)
		{
		case TYPE_NONE:
			None();
			break;
		case TYPE_SMOKE:
			Smoke();
			break;
		case TYPE_SPRAY:
			Spray();
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
	m_nLife --;
	if (m_nLife <= 0)
	{
		m_bUse = false;
	}
}
//============
// 描画処理
//============
void CParticle::Draw(void)
{
}
//
//
//
void CParticle::SetElse(D3DXCOLOR col, int nLife, float fRadius)
{
	m_col = col;
	m_nLife = nLife;
	m_fRadius = fRadius;
}
void CParticle::None(void)
{
	int nCntAppear;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;

	for (nCntAppear = 0; nCntAppear < 5; nCntAppear++)
	{
		pos = m_pos;

		//fAngle = (float)(rand() % 629 - 314) / 100.0f;
		//fLength = (float)(rand() % 10) / 5.0f + 0.3f;

		float theta = D3DXToRadian(rand() % 360);			// 方位角
		float phi = D3DXToRadian(rand() % 180);				// 仰角
		float fLength = (float)(rand() % 20) / 10.0f + 0.5f;

		//move.x = sinf(fAngle) * fLength;
		//move.y = cosf(fAngle) * fLength;
		move.x = sinf(phi) * cosf(theta) * fLength;
		move.y = cosf(phi) * fLength;
		move.z = sinf(phi) * sinf(theta) * fLength;

		col = m_col;

		fRadius = m_fRadius;
		nLife = 30;

		CEffect::Create(pos, m_rot, move, col, nLife, fRadius, 6);
	}
}
void CParticle::Smoke(void)
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	
	pos = m_pos;

	float xRand = ((float)(rand() % 200) - 100.0f) / 75.0f;
	float zRand = ((float)(rand() % 200) - 100.0f) / 75.0f;
	float ySpeed = ((float)(rand() % 50) / 100.0f) + 3.0f;

	move = D3DXVECTOR3(xRand, ySpeed, zRand);

	// 色・サイズ・寿命の設定
	col = m_col;
	float fRadius = m_fRadius;
	int nLife = 40 + rand() % 20;

	CEffect::Create(pos, m_rot, move, col, nLife, fRadius, 5);
}

void CParticle::Spray(void)
{
	D3DXVECTOR3 pos = m_pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col = m_col;

	float angle = m_rot.y;

	for (int i = 0; i < 36; i++)
	{
		float spread = D3DXToRadian((rand() % 200 - 100) / 10.0f);
		float dir = angle + spread;

		float speed = 3.0f + (rand() % 150) / 50.0f;

		move.x = -sinf(dir) * speed;
		move.y = ((rand() % 100) - 50) / 200.0f;
		move.z = -cosf(dir) * speed;

		// 粒子の半径
		float fRadius = m_fRadius * 0.8f + ((rand() % 10) / 30.0f);

		// 寿命短め（消火器の粉はすぐ消える）
		int nLife = 12 + rand() % 5;

		CEffect::Create(pos, m_rot, move, col, nLife, fRadius, 6);
	}
}