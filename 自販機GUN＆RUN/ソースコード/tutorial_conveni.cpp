//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "tutorial_conveni.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"
#include "arrow.h"

//==================
// コンストラクタ
//==================
CTutorialConveni::CTutorialConveni(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = true;
	m_bClear = false;
	m_type = TYPE_STOCK;

	m_nIdxTex = 0;
}
//================
// デストラクタ
//================
CTutorialConveni::~CTutorialConveni()
{
}
//===========
// 生成処理
//===========
CTutorialConveni* CTutorialConveni::Create()
{
	CTutorialConveni* pObject2D;
	pObject2D = new CTutorialConveni;
	//初期化
	pObject2D->Init();
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CTutorialConveni::Init()
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//値を代入
	m_fWidth = SCREEN_WIDTH / 1.5f;
	m_fHeight = SCREEN_HEIGHT / 1.5f;

	m_posStay = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);;
	m_posOut = m_posStay + D3DXVECTOR3(-m_fWidth * 2, 0.0f, 0.0f);
	m_pos = m_posOut;

	m_state = STATE_SLIDEIN;

	m_bUse = true;
	m_bClear = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	const char* apFileName[TYPE_MAX] =
	{
		"data\\TEXTURE\\tutorial_conveni00.jpg",
		"data\\TEXTURE\\tutorial_conveni01.jpg",
		"data\\TEXTURE\\tutorial_conveni02.jpg",
	};

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apFileName[nCnt] = apFileName[nCnt];
	}

	CObject2D::Init(m_apFileName[m_type], m_posOut, m_rot, m_fWidth, m_fHeight);
	
	m_pSkip = CObject2D::Create("data\\TEXTURE\\skip00.png", D3DXVECTOR3(m_posStay.x - (m_fWidth / 1.75f), m_posStay.y + (m_fHeight / 1.75f), m_posStay.z), m_rot, m_fWidth / 3, m_fHeight / 10, 9);
	m_pClick = CClick::Create("data\\TEXTURE\\next00.png", D3DXVECTOR3(SCREEN_WIDTH - (CLICK_SIZE_X / 1.5f), SCREEN_HEIGHT - (CLICK_SIZE_Y / 1.5f), 0.0f), CLICK_SIZE_X, CLICK_SIZE_Y);
	m_pClick->SetUse(true);

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(m_apFileName[m_type]);
	CObject2D::SetIdxTex(m_nIdxTex);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	CGame::SetMode(CGame::MODE_TUTORIAL_CONVENI);
	CManager::SetPause(true);

	return S_OK;
}
//============
// 終了処理
//============
void CTutorialConveni::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CTutorialConveni::Update(void)
{
	const float fSpeed = 100.0f;

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	if (m_bUse == true)
	{
		if (pInputKey->GetTrigger(DIK_R) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_BACK) == true)
		{
			m_state = STATE_SLIDEOUT;
			m_type = TYPE_ENEMY;
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
					m_bUse = false;
				}
				m_bClear = false;
			}
			break;
		default:
			break;
		}

		CObject2D::SetPos(m_pos);

		m_pClick->Update();
	}
	else if (m_bUse == false)
	{
		CStartUI::Create("data\\TEXTURE\\game_start00.png", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), STARTUI_SIZE_X, STARTUI_SIZE_Y);
		
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_START);

		CGame::SetMode(CGame::MODE_PLAY);

		CPlayer* pPlayer = CGame::GetPlayer();
		pPlayer->GetCntSystem()->Reset();

		CObject2D::SetUse(false);
		m_pSkip->SetUse(false);
		m_pClick->SetUse(false);

		CManager::SetPause(false);

		const float fWidth = 250.0f;
		const float fHeight = 100.0f;

		CObject2D::Create("data\\TEXTURE\\tutorial_play00.png", D3DXVECTOR3(fWidth / 2, SCREEN_HEIGHT - (fHeight / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 8);

		Uninit();
	}
}
//============
// 描画処理
//============
void CTutorialConveni::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();

		m_pClick->Draw();
	}
}
//==================
// クリアチェック
//==================
void CTutorialConveni::CheckClear(void)
{
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	if (pInputMouse->GetTrigger(0) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_A) == true)
	{
		m_bClear = true;
	}
}