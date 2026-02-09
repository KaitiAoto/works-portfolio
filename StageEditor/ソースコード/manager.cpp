//================================
//
// マネージャー処理[manager.cpp]
// Author:kaiti
//
//================================
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "player.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "collision.h"
#include "debugproc.h"
#include "loadstage.h"
#include "modelset.h"
#include "savestage.h"
//静的メンバ変数
CRenderer* CManager::m_pRenderer = NULL;
CInputKey* CManager::m_pInputKey = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CPlayer* CManager::m_pPlayer = NULL;
CTexture* CManager::m_pTex = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CObject3D* CManager::m_pObj3D = NULL;
//CRestock* CManager::m_pRestock = NULL;
CCollision* CManager::m_pColl = NULL;
CDebugProc* CManager::m_pDebug = NULL;
CLoadStage* CManager::m_pStage = NULL;
CModelSet* CManager::m_pModelSet = NULL;
CSaveStage* CManager::m_pSaveStage = NULL;

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
	//必要オブジェクト生成
	CreateAll();

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

	//当たり判定生成
	if (m_pColl == NULL)
	{
		m_pColl = new CCollision;
	}

	//デバッグ生成
	if (m_pDebug == NULL)
	{
		m_pDebug = new CDebugProc;
		m_pDebug->Init();
	}

	return S_OK;
}
//=============
// 全生成処理
//=============
void CManager::CreateAll(void)
{
	//プレイヤー
	m_pModelSet = CModelSet::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CModelSet::TYPE_MODEL);

	if (m_pSaveStage == NULL)
	{
		m_pSaveStage = new CSaveStage;
	}

	if (m_pStage == NULL)
	{
		m_pStage = new CLoadStage;

		m_pStage->Load("data\\STAGE\\model01.txt");
	}

}
//=============
// 終了処理
//=============
void CManager::Uninit(void)
{
	if (m_pStage != NULL)
	{
		m_pStage->Unload();
		delete m_pStage;
		m_pStage = nullptr;
	}
	if (m_pSaveStage != NULL)
	{
		m_pSaveStage->Unload();
		delete m_pSaveStage;
		m_pSaveStage = nullptr;
	}

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
	//
	if (m_pDebug != NULL)
	{
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	//
	if (m_pColl != NULL)
	{
		delete m_pColl;
		m_pColl = nullptr;
	}

	//テクスチャ破棄
	if (m_pTex != NULL)
	{
		//テクスチャ終了
		m_pTex->Unload();

		delete m_pTex;
		m_pTex = NULL;
	}

	//カメラ破棄
	if (m_pCamera != NULL)
	{
		//カメラ終了
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = NULL;
	}
	//ライト破棄
	if (m_pLight != NULL)
	{
		//ライト終了
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	//キー入力破棄
	if (m_pInputKey != NULL)
	{
		//キー入力終了
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


	//レンダラー破棄
	if (m_pRenderer != NULL)
	{
		//レンダラー終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

}
//=======================
// マネージャーの更新
//=======================
void CManager::Update(void)
{
	if (m_pInputKey != nullptr)
	{
		//キー入力更新
		m_pInputKey->Update();
	}
	if (m_pInputMouse != nullptr)
	{
		m_pInputMouse->Update();
	}

	//レンダラー更新
	m_pRenderer->Update();

	if (m_pCamera != nullptr)
	{
		//カメラ更新
		m_pCamera->Update();
	}
}
//=======================
// マネージャーの描画
//=======================
void CManager::Draw(void)
{
	int n = CStage::GetCntModel();
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("配置モデル数：%d", n);

	//レンダラー描画
	m_pRenderer->Draw();

}