//==============================
//
//  タイマー処理[.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge_reset.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "object2D.h"
#include "input.h"
//==================
// コンストラクタ
//==================
CResetGauge::CResetGauge(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
}
//================
// デストラクタ
//================
CResetGauge::~CResetGauge()
{
}
//===========
// 生成処理
//===========
CResetGauge* CResetGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CResetGauge* pGause;
	//
	pGause = new CResetGauge;
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
HRESULT CResetGauge::Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	m_pos = pos;
	m_Base = base;
	m_fHeight = fHeight;
	m_col = col;
	m_bUse = false;
	m_pGauge = CGauge::Create(m_pos, m_Base, m_fHeight, m_col);
	m_pObj2D = CObject2D::Create("data\\TEXTURE\\reset00.png", D3DXVECTOR3(m_pos.x + 150.0f, m_pos.y, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 250, 50, 7);

	//オブジェクトの種類設定
	//SetObjType(TYPE_GAUGE);

	return S_OK;
}
//==========
// 終了処理
//==========
void CResetGauge::Uninit(void)
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
void CResetGauge::Update(void)
{
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	////キー取得
	//CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pPad = CManager::GetInputPad();

	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	int nCntContents = pPlayer->GetContents();

	if (nCntContents > 0)
	{//中身あり
		if ((pInputMouse->GetPress(0) == true && pInputMouse->GetPress(1) == true) || pPad->GetPress(CInputPad::JOYKEY_Y) == true)
		{//Ｆ長押し
			m_bUse = true;
			m_nCntReset++;
			m_Base += 0.45f;
			if (m_nCntReset >= 30)
			{//一定時間経過で中身を０に
				pPlayer->ClearContents();
				m_nCntReset = 0;
			}
		}
		else
		{
			m_nCntReset = 0;
			m_Base = 0;
			m_bUse = false;
		}
	}
	else
	{
		m_nCntReset = 0;
		m_Base = 0;
		m_bUse = false;
	}

	m_pObj2D->SetUse(m_bUse);
	m_pGauge->SetBase(m_Base);
	m_pGauge->Set();
}
//===========
// 描画処理
//===========
void CResetGauge::Draw(void)
{
	if (m_bUse == true)
	{
		m_pGauge->Draw();
	}
}