//===============================================
//
//　カメラ処理[camera.cpp]
//　Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "camera.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//===============================================
// コンストラクタ
//===============================================
CCamera::CCamera(void)
{
	// 値をクリア
	m_posV = VECTOR3_NULL;
	m_posR = VECTOR3_NULL;	
	m_posVDest = VECTOR3_NULL;
	m_posRDest = VECTOR3_NULL;
	m_vecU = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_posVOffSet = VECTOR3_NULL;
	m_fDistance = 0.0f;
	m_mtxProjection = {};
	m_mtxView = {};
	m_bShake = false;
}
//===============================================
// デストラクタ
//===============================================
CCamera::~CCamera()
{
}
//===============================================
// 初期化
//===============================================
HRESULT CCamera::Init(void)
{
	// メンバ変数の初期化
	// 視点・注視点・上方向を設定する
	m_posV = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);
	m_posR = VECTOR3_NULL;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_posVOffSet = m_posV;								// 視点のオフセット
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// 角度
	// 視点と注視点の距離
	m_fDistance = sqrtf(((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x))
					  + ((m_posV.y - m_posR.y) * (m_posV.y - m_posR.y))
					  + ((m_posV.z - m_posR.z) * (m_posV.z - m_posR.z)));	
	m_bMove = false;
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CCamera::Uninit(void)
{
}
//===============================================
// 更新処理（ゲーム）
//===============================================
void CCamera::Update(void)
{
	// デバッグフォント取得
	CDebugProc* pDegub = CManager::GetDebug();
	// キー取得
	CInputKey* pInputKey = CManager::GetInputKey();

#ifdef _DEBUG

	if (pInputKey->GetTrigger(DIK_F2) == true)
	{//bUse切り替え
		m_bMove = !m_bMove;
	}
#endif // DEBUG

	// 見下ろし固定カメラ
	if (m_bMove == false)
	{
		m_posV = D3DXVECTOR3(0.0f, 1200.0f, -1100.0f);
		m_posR = VECTOR3_NULL;
		m_rot = VECTOR3_NULL;	// 角度

		// 視点と注視点の距離
		m_fDistance = sqrtf(((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x))
			+ ((m_posV.y - m_posR.y) * (m_posV.y - m_posR.y))
			+ ((m_posV.z - m_posR.z) * (m_posV.z - m_posR.z)));
	}
	else if (m_bMove == true)
	{
		// マウス取得
		CInputMouse* pInputMouse = CManager::GetInputMouse();
		// パッド取得
		CInputPad* pInputPad = CManager::GetInputPad();

		// カメラアップ
		int wheel = 0;
		wheel = pInputMouse->GetWheel();
		if (wheel > 0)
		{// ↑
			m_fDistance -= 30.0f;
		}
		else if (wheel < 0)
		{// ↓
			m_fDistance += 30.0f;
		}

		// マウスの左右移動で視点旋回
		if (pInputMouse != nullptr)
		{
			int mouseMoveX = pInputMouse->GetRelX();

			const float sensitivity = 0.0009f;

			m_rot.y += mouseMoveX * sensitivity;

			int mouseMoveY = pInputMouse->GetRelY();

			const float sensitivityY = 0.05f;

			m_posV.y += mouseMoveY * sensitivityY;

			if (m_posV.y <= 40.0f)
			{
				m_posV.y = 40.0f;
			}
		}

		if (pInputPad != nullptr)
		{
			const float sensitivity = 0.1f;
			const float sensitivityY = 3.0f;
			const float rx = pInputPad->GetRightStickX();
			const float ry = pInputPad->GetRightStickY();

			// デッドゾーン処理
			const float deadZone = 0.2f;
			if (fabsf(rx) > deadZone)
			{
				m_rot.y += rx * sensitivity;
			}
			if (fabsf(ry) > deadZone)
			{
				m_posV.y += ry * sensitivityY;

			}
		}

		// 視点の旋回
		if (pInputKey->GetPress(DIK_Q) == true)
		{
			m_rot.y += 0.05f;
		}
		else if (pInputKey->GetPress(DIK_E) == true)
		{
			m_rot.y -= 0.05f;
		}
		else if (pInputKey->GetPress(DIK_Y) == true)
		{
			m_posV.y += 5.0f;
		}
		else if (pInputKey->GetPress(DIK_N) == true)
		{
			m_posV.y -= 5.0f;
		}

		// 角度の正規化
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

	}
	UpdateShake();

	pDegub->Print("カメラの角度：(%.1f, %.1f, %.1f)", m_rot.x, m_rot.y, m_rot.z);
	pDegub->Print("カメラの視点：(%.1f, %.1f, %.1f)", m_posV.x, m_posV.y, m_posV.z);
	pDegub->Print("カメラの注視点：(%.1f, %.1f, %.1f)", m_posR.x, m_posR.y, m_posR.z);
	pDegub->Print("カメラのベクトル：(%.1f, %.1f, %.1f)", m_vecU.x, m_vecU.y, m_vecU.z);
	pDegub->Print("視点と注視点の距離：%.1f", m_fDistance);
}
//===============================================
// 画面揺れを設定
//===============================================
void CCamera::SetShake(float fShakeX, float fShakeY, int nTime)
{
	m_nShakeTime = nTime;
	m_fMaxShakeX = fShakeX;
	m_fMaxShakeY = fShakeY;
	m_bShake = true;
}
//===============================================
// 画面揺れを更新
//===============================================
void CCamera::UpdateShake(void)
{
	if (m_bShake == true)
	{
		m_nShakeTime--;
		if (m_nShakeTime > 0)
		{
			float offsetX = ((rand() % 200) - 100) / 100.0f * m_fMaxShakeX;
			float offsetY = ((rand() % 200) - 100) / 100.0f * m_fMaxShakeY;

			m_posV.x += offsetX;
			m_posV.y += offsetY;
		}
		else if(m_nShakeTime <= 0)
		{
			m_nShakeTime = 0;
			m_bShake = false;
		}
	}
}
//===============================================
// 更新処理（タイトル・リザルト・ランキング）
//===============================================
void CCamera::UpdateMenu(void)
{
	m_fDistance = 800.0f;//

	m_posR = VECTOR3_NULL;
	m_posV.y = 200.0f;

	//回転
	m_rot.y += 0.003f;

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
	pDegub->Print("視点と注視点の距離：%.1f", m_fDistance);
}
//===============================================
// カメラの設定
//===============================================
void CCamera::Set(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
					   &m_posV,
					   &m_posR,
					   &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f), // 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // アスペクト比
		10.0f,
		//5000.0f);
		10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

