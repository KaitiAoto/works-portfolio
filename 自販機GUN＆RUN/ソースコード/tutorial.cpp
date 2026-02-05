//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "tutorial.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"
#include "arrow.h"

//==================
// コンストラクタ
//==================
CTutorial::CTutorial(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = true;
	m_bClear = false;
	m_type = TYPE_MOVE;

	m_nIdxTex = 0;
}
//================
// デストラクタ
//================
CTutorial::~CTutorial()
{
}
//===========
// 生成処理
//===========
CTutorial* CTutorial::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CTutorial* pObject2D;
	pObject2D = new CTutorial;
	//初期化
	pObject2D->Init(pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//値を代入
	m_posStay = pos;
	m_posOut = pos + D3DXVECTOR3(-fWidth, 0.0f, 0.0f);

	m_pos = m_posOut;

	m_state = STATE_SLIDEIN;

	m_bUse = true;
	m_bClear = false;
	m_nTime = 0;

	m_nCntTime = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	const char* apFileName[TYPE_MAX] =
	{
		"data\\TEXTURE\\tutorial00.jpg",
		"data\\TEXTURE\\tutorial01.jpg",
		"data\\TEXTURE\\tutorial03.jpg",
		"data\\TEXTURE\\tutorial04.jpg",
		"data\\TEXTURE\\tutorial08.jpg",

		"data\\TEXTURE\\tutorial06.jpg",
		"data\\TEXTURE\\tutorial07.jpg",
		
		"data\\TEXTURE\\tutorial05.jpg",
	};

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apFileName[nCnt] = apFileName[nCnt];
	}

	CObject2D::Init(m_apFileName[m_type], pos, m_rot, fWidth, fHeight);
	
	m_pBack = CObject2D::Create(NULL, pos, m_rot, fWidth * 1.05f, fHeight * 1.05f, 7);

	m_pSkip = CObject2D::Create("data\\TEXTURE\\skip00.png", D3DXVECTOR3(pos.x, pos.y + fHeight / 1.75f, pos.z), m_rot, fWidth * 1.25f, fHeight / 6, 7);

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(m_apFileName[m_type]);
	CObject2D::SetIdxTex(m_nIdxTex);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CTutorial::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CTutorial::Update(void)
{
	const float fSpeed = 8.0f;
	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();

	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	if (pInputKey->GetTrigger(DIK_R) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_BACK) == true)
	{
		m_state = STATE_SLIDEOUT;
		m_type = TYPE_MATCHUP;
		m_bClear = true;
	}

	switch (m_state)
	{
	case STATE_STAY:
		CheckClear();

		if (m_bClear == true)
		{
			m_state = STATE_SLIDEOUT;
		}
		break;
	case STATE_SLIDEIN:
		if (m_bUse == true)
		{
			m_pos.x += fSpeed;
			if (m_pos.x >= m_posStay.x)
			{
				m_state = STATE_STAY;
				m_pos = m_posStay;
			}
		}
		break;
	case STATE_SLIDEOUT:
		m_pos.x -= fSpeed;
		if (m_pos.x <= m_posOut.x)
		{
			m_pos = m_posOut;

			//テクスチャを次へ
			m_type = static_cast<TYPE>(m_type + 1);
			if (m_type != TYPE_MAX)
			{
				m_nIdxTex = pTex->Register(m_apFileName[m_type]);
				CObject2D::SetIdxTex(m_nIdxTex);

				m_state = STATE_SLIDEIN;
			}
			else
			{
				CGame::GetStart()->SetDraw(false);

				m_bUse = false;
			}
			m_bClear = false;
		}
		break;
	default:
		break;
	}

	CObject2D::SetPos(m_pos);
	m_pBack->SetPos(m_pos);
	
	m_nCntTime++;
	BackBlink();

	if (m_bUse == false)
	{
		CPlayer* pPlayer = CGame::GetPlayer();
		D3DXVECTOR3 pos = pPlayer->GetPos();
		CArrow::Create({ pos.x,pos.y + (pPlayer->GetSize().y / 4),pos.z });
		
		CObject2D::SetUse(false);
		m_pBack->SetUse(false);
		m_pSkip->SetUse(false);
		Uninit();
	}
}
//============
// 描画処理
//============
void CTutorial::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}
//==============
// 背景点滅
//==============
void CTutorial::BackBlink(void)
{
	// 点滅する速さ
	const float Speed = 0.02f;

	m_col.a = 1.0f * fabsf(sinf(Speed * m_nCntTime));

	// 色を設定
	m_pBack->SetColor(m_col);
}
//==================
// クリアチェック
//==================
void CTutorial::CheckClear(void)
{
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	CPlayer* pPlayer = CGame::GetPlayer();

	switch (m_type)
	{
	case TYPE_MOVE:
	{
		//プレイヤー移動でクリア
		if(pPlayer->GetbMove())
		{
			m_bClear = true;
		}
		break;
	}
	case TYPE_CAMERA:
	{
		//カメラ操作でクリア
		if (pInputMouse->GetRelX() != 0)
		{
			m_bClear = true;
		}

		float rx = pInputPad->GetRightStickX(); // 横
		float ry = pInputPad->GetRightStickY(); // 縦

		// デッドゾーン処理（わずかな傾きによる誤動作防止）
		const float deadZone = 0.2f;
		if (fabsf(rx) > deadZone || fabsf(ry) > deadZone)
		{
			m_bClear = true;
		}

		break;
	}
	case TYPE_CHANGE:
		if (pInputMouse->GetPress(0) == false)
		{
			if (pInputMouse->GetTrigger(1) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_R1) == true)
			{
				m_bClear = true;
			}
		}
		break;
	case TYPE_HOLD:
		//時間経過でクリア
		m_nTime++;
		if (m_nTime >= 180)
		{
			m_bClear = true;
			m_nTime = 0;
		}
		break;
	case TYPE_MATCHUP:
		//時間経過でクリア
		m_nTime++;
		if (m_nTime >= 180)
		{
			m_bClear = true;
			m_nTime = 0;
		}
		break;
	default:
		break;
	}

}
