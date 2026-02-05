//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "startUI.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CStartUI::CStartUI(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bStop = false;
	m_nTime = 0;
	m_bUse = true;
}
//================
// デストラクタ
//================
CStartUI::~CStartUI()
{
}
//===========
// 生成処理
//===========
CStartUI* CStartUI::Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CStartUI* pObject2D;
	pObject2D = new CStartUI;
	//初期化
	pObject2D->Init(pTexName, pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CStartUI::Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//値を代入
	m_pos = D3DXVECTOR3(-fWidth, pos.y, pos.z);
	m_posStay = pos;
	m_bStop = false;
	m_nTime = 0;

	CObject2D::Init(pTexName, m_pos, m_rot, fWidth, fHeight);


	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CStartUI::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CStartUI::Update(void)
{
	if (m_bStop == false)
	{
		Move();
	}
	else
	{
		m_nTime++;
		if (m_nTime >= 240)
		{
			m_bStop = false;
		}
	}

	if (m_bUse == false)
	{
		Uninit();
	}
}
//============
// 描画処理
//============
void CStartUI::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (m_bUse == true)
	{
		//ALPHAテストの設定
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		CObject2D::Draw();

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void CStartUI::Move(void)
{
	m_pos.x += 30.0f;

	if (m_nTime < 120)
	{
		if (m_pos.x >= m_posStay.x)
		{
			m_pos = m_posStay;
			m_bStop = true;
		}
	}

	CObject2D::SetPos(m_pos);

	if (m_pos.x >= SCREEN_WIDTH + STARTUI_SIZE_X / 2)
	{
		m_bUse = false;
	}
}
