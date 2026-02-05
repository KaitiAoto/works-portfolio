//================================
//
// マネージャー処理[manager.cpp]
// Author : Kaiti Aoto
//
//================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "debugproc.h"
#include "tutorial_conveni.h"
//静的メンバ変数
CRenderer* CManager::m_pRenderer = NULL;
CInputKey* CManager::m_pInputKey = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CInputPad* CManager::m_pInputPad = NULL;
CTexture* CManager::m_pTex = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CSound* CManager::m_pSound = NULL;
CDebugProc* CManager::m_pDebug = NULL;
CScene* CManager::m_pScene = NULL;
CFade* CManager::m_pFade = NULL;

bool CManager::m_bPause = false;
//==================
// コンストラクタ
//==================
CManager::CManager()
{
	
}
//================
// デストラクタ
//================
CManager::~CManager()
{
}
//=======================
// マネージャーの初期化
//=======================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//全ポインタ生成
	CreatePointa(hInstance, hWnd, bWindow);

	//SetMode(CScene::MODE_TITLE);

	return S_OK;
}
//================
// ポインタ生成
//================
HRESULT CManager::CreatePointa(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラー生成
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;
	}
	//レンダラーの初期化
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	//キー入力生成
	if (m_pInputKey == NULL)
	{
		m_pInputKey = new CInputKey;
	}
	//キー入力の初期化
	if (FAILED(m_pInputKey->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キー入力生成
	if (m_pInputMouse == NULL)
	{
		m_pInputMouse = new CInputMouse;
	}
	//キー入力の初期化
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キー入力生成
	if (m_pInputPad == NULL)
	{
		m_pInputPad = new CInputPad;
	}
	//キー入力の初期化
	if (FAILED(m_pInputPad->Init(hInstance)))
	{
		return E_FAIL;
	}

	//サウンド生成
	if (m_pSound == NULL)
	{
		m_pSound = new CSound;
	}
	//サウンドの初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	//カメラ生成
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
		m_pCamera->Init();
	}

	//ライト生成
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
		m_pLight->Init();
	}

	//テクスチャ生成
	if (m_pTex == NULL)
	{
		m_pTex = new CTexture;
		m_pTex->Load();
	}

	//デバッグ生成
	if (m_pDebug == NULL)
	{
		m_pDebug = new CDebugProc;
		m_pDebug->Init();
	}

	// フェード生成
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(CScene::MODE_TITLE);
		m_pFade->Set(CScene::MODE_TITLE);
	}

	return S_OK;
}
//==================
// モードの設定
//==================
void CManager::SetMode(CScene::MODE mode)
{
	if (mode != CScene::MODE_RANKING)
	{// ランキング以外は音を止める
		m_pSound->Stop();
	}

	// スコア保存用変数
	int nBreakCnt = 0;
	int nTotalScore = 0;

	if (m_pScene != nullptr)
	{// NULLチェック
		// 現在がゲームで次がリザルト
		if (mode == CScene::MODE_RESULT && m_pScene->GetMode() == CScene::MODE_GAME)
		{
			nBreakCnt = CGame::GetBreakCnt()->GetScore();		// 破壊スコア保存
			nTotalScore = CGame::GetTotalScore()->GetScore();	// トータルスコア保存
			nTotalScore += nBreakCnt * 10000;
		}
		// 現在がリザルトで次がランキング
		else if (mode == CScene::MODE_RANKING && m_pScene->GetMode() == CScene::MODE_RESULT)
		{
			nBreakCnt = CResult::GetBreakCnt()->GetScore();		// 破壊スコア保存
			nTotalScore = CResult::GetTotalScore()->GetScore();	// トータルスコア保存
		}
		// シーンの終了処理
		m_pScene->Uninit();
	}
	// オブジェクト総破棄
	CObject::ReleaseAll();

	// 新しいシーンを生成
	m_pScene = CScene::Create(mode);
	// シーンの初期化
	m_pScene->Init();
	
	// 現在がランキング
	if (mode == CScene::MODE_RANKING)
	{
		CRanking::SetNowScore(nBreakCnt, nTotalScore);	// 今回のスコアを保存
	}
	if (m_pScene != nullptr && m_pScene->GetMode() == CScene::MODE_RESULT)
	{//NULLチェック＆現在がリザルト
		// スコア代入
		CResult::GetBreakCnt()->Set(nBreakCnt);
		CResult::GetTotalScore()->Set(nTotalScore);
	}

	// カメラ初期化
	m_pCamera->Init();
}
//=============
// 終了処理
//=============
void CManager::Uninit(void)
{
	//オブジェクト破棄
	CObject::ReleaseAll();
	//全ポインタ破棄
	ReleasePointa();
}
//===============
// ポインタ破棄
//===============
void CManager::ReleasePointa(void)
{
	//レンダラー破棄（最後に生成されたものを最初に破棄）
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//キー入力破棄
	if (m_pInputKey != NULL)
	{
		m_pInputKey->Uninit();
		delete m_pInputKey;
		m_pInputKey = NULL;
	}
	//キー入力破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	//キー入力破棄
	if (m_pInputPad != NULL)
	{
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = NULL;
	}

	//サウンド破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//ライト破棄
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//カメラ破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//テクスチャ破棄
	if (m_pTex != NULL)
	{
		m_pTex->Unload();
		delete m_pTex;
		m_pTex = NULL;
	}
	//デバッグ
	if (m_pDebug != NULL)
	{
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

}
//=======================
// マネージャーの更新
//=======================
void CManager::Update(void)
{
	//CalculateFPS();

	if (m_bPause == false)
	{
		//レンダラー更新
		m_pRenderer->Update();

		if (m_pCamera != nullptr)
		{
			if (m_pScene != nullptr)
			{
				if (m_pScene->GetMode() == CScene::MODE_GAME)
				{
					//カメラ更新
					m_pCamera->Update();
				}
				else
				{
					m_pCamera->UpdateMenu();
				}
			}
		}
	}
	else
	{
		if (CGame::GetMode() != CGame::MODE_TUTORIAL_CONVENI)
		{
			CPause* pPause = CGame::GetPause();
			pPause->SetDraw(true);
			pPause->Update();
		}
		else if (CGame::GetMode() == CGame::MODE_TUTORIAL_CONVENI)
		{
			CTutorialConveni* tuto = CGame::GetTutoConveni();
			tuto->Update();
		}
	}

	if (m_pInputKey != nullptr)
	{
		//キー入力更新
		m_pInputKey->Update();
	}
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Update();
	}
	if (m_pInputPad != nullptr)
	{
		m_pInputPad->Update();
	}

	m_pFade->Update();
}
//=======================
// マネージャーの描画
//=======================
void CManager::Draw(void)
{
	//レンダラー描画
	m_pRenderer->Draw();

	//DrawFPS();
}
