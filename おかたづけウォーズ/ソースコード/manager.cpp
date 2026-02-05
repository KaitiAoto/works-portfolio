//===============================================
//
// マネージャー処理[manager.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
#include "debugproc.h"

//静的メンバ変数
CRenderer* CManager::m_pRenderer = nullptr;
CInputKey* CManager::m_pInputKey = nullptr;
CInputMouse* CManager::m_pInputMouse = nullptr;
CInputPad* CManager::m_pInputPad = nullptr;
CTexture* CManager::m_pTex = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;
CSound* CManager::m_pSound = nullptr;
CDebugProc* CManager::m_pDebug = nullptr;
CScene* CManager::m_pScene = nullptr;
CFade* CManager::m_pFade = nullptr;
CColl* CManager::m_pColl = nullptr;
bool CManager::m_bPause = false;

//===============================================
// コンストラクタ
//===============================================
CManager::CManager()
{
	
}
//===============================================
// デストラクタ
//===============================================
CManager::~CManager()
{
}
//===============================================
// マネージャーの初期化
//===============================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//全ポインタ生成
	CreatePointa(hInstance, hWnd, bWindow);

	//SetMode(CScene::MODE_TITLE);

	return S_OK;
}
//===============================================
// ポインタ生成
//===============================================
HRESULT CManager::CreatePointa(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラー生成
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;
	}
	//レンダラーの初期化
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	//キー入力生成
	if (m_pInputKey == nullptr)
	{
		m_pInputKey = new CInputKey;
	}
	//キー入力の初期化
	if (FAILED(m_pInputKey->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キー入力生成
	if (m_pInputMouse == nullptr)
	{
		m_pInputMouse = new CInputMouse;
	}
	//キー入力の初期化
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//キー入力生成
	if (m_pInputPad == nullptr)
	{
		m_pInputPad = new CInputPad;
	}
	//キー入力の初期化
	if (FAILED(m_pInputPad->Init(hInstance)))
	{
		return E_FAIL;
	}

	//サウンド生成
	if (m_pSound == nullptr)
	{
		m_pSound = new CSound;
	}
	//サウンドの初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	//カメラ生成
	if (m_pCamera == nullptr)
	{
		m_pCamera = new CCamera;
		m_pCamera->Init();
	}

	//ライト生成
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight;
		m_pLight->Init();
	}

	//テクスチャ生成
	if (m_pTex == nullptr)
	{
		m_pTex = new CTexture;
		m_pTex->Load();
	}

	//デバッグ生成
	if (m_pDebug == nullptr)
	{
		m_pDebug = new CDebugProc;
		m_pDebug->Init();
	}

	// 当たり判定生成
	if (m_pColl == nullptr)
	{
		m_pColl = new CColl;
	}

	// フェード生成
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(CScene::MODE_TITLE);
		m_pFade->Set(CScene::MODE_TITLE);
	}

	return S_OK;
}
//===============================================
// モードの設定
//===============================================
void CManager::SetMode(CScene::MODE mode)
{
	if (mode != CScene::MODE_RANKING)
	{// ランキング以外は音を止める
		m_pSound->Stop();
	}

	if (m_pScene != nullptr)
	{// NULLチェック
		// シーンの終了処理
		m_pScene->Uninit();
	}
	// オブジェクト総破棄
	CObject::ReleaseAll();

	// 新しいシーンを生成
	m_pScene = CScene::Create(mode);
	// シーンの初期化
	m_pScene->Init();	

	// カメラ初期化
	m_pCamera->Init();
}
//===============================================
// 終了処理
//===============================================
void CManager::Uninit(void)
{
	//オブジェクト破棄
	CObject::ReleaseAll();
	//全ポインタ破棄
	ReleasePointa();
}
//===============================================
// ポインタ破棄
//===============================================
void CManager::ReleasePointa(void)
{
	// レンダラー破棄（最後に生成されたものを最初に破棄）
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// キー入力破棄
	if (m_pInputKey != nullptr)
	{
		m_pInputKey->Uninit();
		delete m_pInputKey;
		m_pInputKey = nullptr;
	}
	// マウス入力破棄
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}
	// パッド入力破棄
	if (m_pInputPad != nullptr)
	{
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = nullptr;
	}

	// サウンド破棄
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	// ライト破棄
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// カメラ破棄
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// テクスチャ破棄
	if (m_pTex != nullptr)
	{
		m_pTex->Unload();
		delete m_pTex;
		m_pTex = nullptr;
	}

	// デバッグ
	if (m_pDebug != nullptr)
	{
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	// 当たり判定破棄
	if (m_pColl != nullptr)
	{
		delete m_pColl;
		m_pColl = nullptr;
	}

	// フェード破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

}
//===============================================
// マネージャーの更新
//===============================================
void CManager::Update(void)
{
	// ポーズじゃない
	if (m_bPause == false)
	{
		//レンダラー更新
		m_pRenderer->Update();

		// NULLチェック
		if (m_pCamera != nullptr && m_pScene != nullptr)
		{
			// ゲームorチュートリアルなら
			if (m_pScene->GetMode() == CScene::MODE_GAME || m_pScene->GetMode() == CScene::MODE_TUTORIAL)
			{
				//カメラ更新
				m_pCamera->Update();
			}
			else
			{
				// メニュー用カメラ更新
				m_pCamera->UpdateMenu();
			}
		}
	}
	// ポーズなら
	else if (m_bPause == true)
	{
		// ポーズ
		CPause* pPause = CGame::GetPause();
		pPause->SetDraw(true);	// 描画する
		pPause->Update();		// 更新
	}

	if (m_pInputKey != nullptr)
	{
		// キー入力更新
		m_pInputKey->Update();
	}
	if (m_pInputMouse != nullptr)
	{
		// マウス入力更新
		m_pInputMouse->Update();
	}
	if (m_pInputPad != nullptr)
	{
		// パッド入力更新
		m_pInputPad->Update();
	}

	// フェードの更新
	m_pFade->Update();
}
//===============================================
// マネージャーの描画
//===============================================
void CManager::Draw(void)
{
	//レンダラー描画
	m_pRenderer->Draw();
}
