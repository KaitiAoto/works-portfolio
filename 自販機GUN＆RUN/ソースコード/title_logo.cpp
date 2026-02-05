//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "title_logo.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "easing.h"

//==================
// コンストラクタ
//==================
CTitleLogo::CTitleLogo(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nTime = 0;
	m_bUse = true;
	m_AnimCnt = 0;
}
//================
// デストラクタ
//================
CTitleLogo::~CTitleLogo()
{
}
//===========
// 生成処理
//===========
CTitleLogo* CTitleLogo::Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CTitleLogo* pObject2D;
	pObject2D = new CTitleLogo;
	//初期化
	pObject2D->Init(pTexName, pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CTitleLogo::Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//値を代入
	m_pos = D3DXVECTOR3(pos.x, -fHeight, pos.z);
	m_posOld = D3DXVECTOR3(pos.x, -fHeight, pos.z);
	m_posStay = pos;
	m_nTime = 0;
	m_bStop = false;

	CObject2D::Init(pTexName, m_pos, m_rot, fWidth, fHeight);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CTitleLogo::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CTitleLogo::Update(void)
{
	if (m_AnimCnt < 75.0f)
	{
		m_AnimCnt++;
	}
	else
	{
		m_bStop = true;
	}
	
	float fOldY = m_pos.y;
	
	m_pos.y = m_posStay.y * CEasing::OutBounce((float)m_AnimCnt / 75.0f);	
	
	float fVelocity = m_pos.y - fOldY;

	if (fVelocity < 0.0f && m_posOld.y > 0.0f)
	{
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_TITLELOGO);
	}

	CObject2D::SetPos(m_pos);

	m_posOld.y = fVelocity;

	if (m_bUse == false)
	{
		Uninit();
	}
}
//============
// 描画処理
//============
void CTitleLogo::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}