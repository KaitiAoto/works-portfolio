//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "restock.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "debugproc.h"
#include "input.h"

//==================
// コンストラクタ
//==================
CRestock::CRestock(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nIdxTex = 0;
	m_bUse = false;
}
//================
// デストラクタ
//================
CRestock::~CRestock()
{
}
//===========
// 生成処理
//===========
CRestock* CRestock::Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CRestock* pObject2D;
	pObject2D = new CRestock;
	//初期化
	pObject2D->Init(pTexName, pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CRestock::Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//値を代入
	m_pos = pos;
	m_bUse = false;

	CObject2D::Init(pTexName, pos, m_rot, fWidth, fHeight);

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(pTexName);

	//オブジェクトの種類設定
	SetObjType(TYPE_RESTOCK);

	return S_OK;
}
//============
// 終了処理
//============
void CRestock::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CRestock::Update(void)
{
	if (m_bUse == true)
	{
		if (m_bRLShake == true)
		{
			m_rot.z += 0.025f;
			if (m_rot.z >= 0.25f)
			{
				m_bRLShake = false;
			}
		}
		else if (m_bRLShake == false)
		{
			m_rot.z -= 0.025f;
			if (m_rot.z <= -0.25f)
			{
				m_bRLShake = true;
			}
		}

		CObject2D::Set(m_pos, m_rot);
	}
}
//============
// 描画処理
//============
void CRestock::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}
//=============
// 設定処理
//=============
void CRestock::Set(int nRestock, CBullet::TYPE type, CVender* pVender)
{
	CDebugProc* pDegub = CManager::GetDebug();

	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	CScoreMana* pTotalScore = CGame::GetTotalScore();

	pDegub->Print("補充可能");
	m_bUse = true;

	if (pInputMouse->GetPress(0) == true || pInputPad->GetR2Press(30) == true)
	{
		//中身補充
		pPlayer->AddContents(2);
		//種類設定
		pPlayer->SetBulletType(type);

		pPlayer->SetCanRestock(true);

		pVender->SubRestock(2);

		pTotalScore->AddScore(1);

		if (pVender->GetRestock() <= 0)
		{
			pPlayer->SetCanRestock(false);
			pPlayer->SetShotTime(0.75f * 5.0f);

			pVender->SetUseRestock(false);
			m_bUse = false;

			CGame::GetFlash()->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f));

			CSound* pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_RESTOCK);

			pPlayer->Heal(PLAYER_LIFE / 4);
		}

		//チュートリアルクリア判定
		if (CGame::GetMode() == CGame::MODE_TUTORIAL)
		{
			CTutorial* pTutorial = CGame::GetTutorial();
			if (pTutorial != nullptr)
			{
				if (pTutorial->GetType() == CTutorial::TYPE_RESTOCK)
				{
					CGame::GetTutorial()->SetClear(true);
				}
			}
		}
	}
	else
	{
	}
}