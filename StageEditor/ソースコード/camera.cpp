//=============================
//
//　カメラ処理[camera.cpp]
//　Author:kaiti
//
//=============================
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "modelset.h"
//================
// コンストラクタ
//================
CCamera::CCamera(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDistance = 0.0f;

	m_mtxProjection = {};
	m_mtxView = {};
}
//================
// デストラクタ
//================
CCamera::~CCamera()
{
}
//=================
// カメラの初期化
//=================
HRESULT CCamera::Init(void)
{
	//視点・注視点・上方向を設定する
	m_posV = D3DXVECTOR3(0.0f, 120.0f, -150.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度

	m_fDistance = sqrtf(((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x))
					  + ((m_posV.y - m_posR.y) * (m_posV.y - m_posR.y))
					  + ((m_posV.z - m_posR.z) * (m_posV.z - m_posR.z)));	//視点と注視点の距離

	m_bUse = false;
	return S_OK;
}
//===================
// カメラの終了処理
//===================
void CCamera::Uninit(void)
{

}
//====================
// カメラの更新処理
//====================
void CCamera::Update(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	//プレイヤー情報取得
	CModelSet* pPlayer = CManager::GetModelSet();
	D3DXVECTOR3 pos = pPlayer->GetPos();
	D3DXVECTOR3 rot = pPlayer->GetRot();


	if (pInputKey->GetTrigger(DIK_F5) == true)
	{//bUse切り替え
		m_bUse = !m_bUse;
	}

	if (m_bUse == true)
	{
		//追従
		m_posRDest.x = pos.x + sinf(rot.x) * (pos.x - m_posR.x);
		//m_posRDest.y = pos.y;
		m_posRDest.z = pos.z + cosf(rot.z) * (pos.z - m_posR.z);

		m_posVDest.x = pos.x + sinf(m_rot.y - D3DX_PI) * m_fDistance;
		m_posVDest.z = pos.z + cosf(m_rot.y - D3DX_PI) * m_fDistance;

		m_posR.x += (m_posRDest.x - m_posR.x) * 0.08f;
		//m_posR.y += (m_posRDest.y - m_posR.y) * 1.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.08f;

		m_posV.x += (m_posVDest.x - m_posV.x) * 0.08f;
		m_posV.z += (m_posVDest.z - m_posV.z) * 0.08f;
	}

	// カメラアップ
	int wheel = 0;
	wheel = pInputMouse->GetWheel();
	if (wheel > 0)
	{// ↑
		m_fDistance -= 30.0f;
		if (m_fDistance <= 150.0f)
		{
			m_fDistance = 150.0f;
		}
	}
	else if (wheel < 0)
	{// ↓
		m_fDistance += 30.0f;
		if (m_fDistance >= 500.0f)
		{
			m_fDistance = 500.0f;
		}
	}

	// マウスの左右移動で視点旋回
	if (pInputMouse != nullptr)
	{
		int mouseMoveX = pInputMouse->GetRelX();

		const float sensitivity = 0.0009f;

		m_rot.y += mouseMoveX * sensitivity;

		int mouseMoveY = pInputMouse->GetRelY();

		const float sensitivityY = 0.1f;

		m_posV.y += mouseMoveY * sensitivityY;

		if (m_posV.y <= -100.0f)
		{
			m_posV.y = -100.0f;
		}
		else if (m_posV.y >= 700.0f)
		{
			m_posV.y = 700.0f;
		}
	}


	// カメラ移動スピード
	const float moveSpeed = 4.0f;

	// 矢印キーで視点（カメラの位置）を移動
	if (pInputKey->GetPress(DIK_UP))
	{
		m_posV.x += sinf(m_rot.y) * moveSpeed;
		m_posV.z += cosf(m_rot.y) * moveSpeed;
		m_posR.x += sinf(m_rot.y) * moveSpeed;
		m_posR.z += cosf(m_rot.y) * moveSpeed;
	}
	if (pInputKey->GetPress(DIK_DOWN))
	{
		m_posV.x -= sinf(m_rot.y) * moveSpeed;
		m_posV.z -= cosf(m_rot.y) * moveSpeed;
		m_posR.x -= sinf(m_rot.y) * moveSpeed;
		m_posR.z -= cosf(m_rot.y) * moveSpeed;
	}
	if (pInputKey->GetPress(DIK_LEFT))
	{
		m_posV.x -= cosf(m_rot.y) * moveSpeed;
		m_posV.z += sinf(m_rot.y) * moveSpeed;
		m_posR.x -= cosf(m_rot.y) * moveSpeed;
		m_posR.z += sinf(m_rot.y) * moveSpeed;
	}
	if (pInputKey->GetPress(DIK_RIGHT))
	{
		m_posV.x += cosf(m_rot.y) * moveSpeed;
		m_posV.z -= sinf(m_rot.y) * moveSpeed;
		m_posR.x += cosf(m_rot.y) * moveSpeed;
		m_posR.z -= sinf(m_rot.y) * moveSpeed;
	}

	//角度の正規化
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posV.x = m_posR.x - sinf(m_rot.y) * m_fDistance;
	m_posV.z = m_posR.z - cosf(m_rot.y) * m_fDistance;

	CDebugProc* pDegub = CManager::GetDebug();

	pDegub->Print("カメラの角度：(%.1f, %.1f, %.1f)", m_rot.x, m_rot.y, m_rot.z);
	pDegub->Print("カメラの視点：(%.1f, %.1f, %.1f)", m_posV.x, m_posV.y, m_posV.z);
}
//================
// カメラの設定
//================
void CCamera::Set(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f), //視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, //アスペクト比
		10.0f,
		5000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
