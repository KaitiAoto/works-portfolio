//==============================
//
// プレイヤー[player.cpp]
// Author:kaiti
//
//==============================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "collision.h"
#include "collision.h"
//静的メンバ変数
bool CPlayer::m_bUse = true;
//==================
// コンストラクタ
//==================
CPlayer::CPlayer(int nPriority):CObject(nPriority)
{
	//値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_type = TYPE_A;

	m_bUse = true;
	m_State = STATE_NONE;
	m_nCntState = 0;
	m_nPriority = nPriority;
	m_bJump = false;
	m_bLanding = false;

	m_nLife = 0;
	m_fSpeed = 0;

	m_nNumModel = 0;
	m_mtxWorld = {};

	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		m_apModel[nCnt] = {};
	}
	m_pMotion = nullptr;

}
//================
// デストラクタ
//================
CPlayer::~CPlayer()
{
}
//===========
// 生成処理
//===========
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CPlayer* pPlayer;
	//インスタンス生成
	pPlayer = new CPlayer;
	//初期化
	if (FAILED(pPlayer->Init(pos, rot)))
	{
		delete pPlayer;
		return nullptr;
	}
	pPlayer->SetType(type);
	return pPlayer;
}
//===============
// 初期化処理
//===============
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//位置・向き設定
	m_pos = pos;
	m_rot = rot;

	//モデル生成
	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] == NULL)
		{
			m_apModel[nCnt] = nullptr;
		}
	}

	//モデル読み込み
	CModel::Load("data\\MOTION\\motion_vending.txt", m_apModel,m_nNumModel);
	//モーション読み込み
	m_pMotion = CMotion::Load("data\\MOTION\\motion_vending.txt", m_apModel);
	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		m_size = m_apModel[nCnt]->SetSize();
	}

	float fRadius = max(m_size.x, max(m_size.y, m_size.z)) * 0.5f;

	m_nLife = PLAYER_LIFE;
	m_fSpeed = PLAYER_SPPE;

	SetObjType(TYPE_PLAYER);//オブジェクトのタイプ
	return S_OK;
}
//============
// 終了処理
//============
void CPlayer::Uninit(void)
{
	m_bUse = false;

	//モデル全終了
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}
	//モーション破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	CObject::Release();
}
//============
// 更新処理
//============
void CPlayer::Update(void)
{
	if (m_bUse == true)
	{
		//行動
		Action();

		//移動
		Move();


		//CEffect::Create(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), 60, EFFECT_SIZE);

		m_nCntState--;
		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			State(STATE_NONE);
		}

		//モーション更新
		if (m_pMotion != nullptr)
		{
			m_pMotion->Update(m_nNumModel, m_apModel);
		}
	}
}
//============
// 描画処理
//============
void CPlayer::Draw(void)
{
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("プレイヤーの位置：%.1f,%.1f,%.1f", m_pos.x, m_pos.y, m_pos.z);
	pDegub->Print("プレイヤーの角度：%.1f,%.1f,%.1f", m_rot.x, m_rot.y, m_rot.z);
	pDegub->Print("プレイヤーのサイズ：%.1f,%.1f,%.1f", m_size.x, m_size.y, m_size.z);
	pDegub->Print("プレイヤーのHP：%d", m_nLife);
	if (m_bLanding == true)
	{
		pDegub->Print("プレイヤーの着地：true");
	}
	else if (m_bLanding == false)
	{
		pDegub->Print("プレイヤーの着地：false");
	}

	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル全描画
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}

}
//===========
// 移動処理
//===========
void CPlayer::Move(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();

	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//キー移動
	if (pInputKey->GetPress(DIK_A) == true)
	{//左
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_move.x += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI / 2;
		}
	}
	if (pInputKey->GetPress(DIK_D) == true)
	{//右
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_move.x -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI / 2;
		}
	}
	if (pInputKey->GetPress(DIK_W) == true)
	{//前
		if (pInputKey->GetPress(DIK_A) == false && pInputKey->GetPress(DIK_D) == false)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI;
		}
	}
	if (pInputKey->GetPress(DIK_S) == true)
	{//後
		if (pInputKey->GetPress(DIK_A) == false && pInputKey->GetPress(DIK_D) == false)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y;
		}
	}

	//ジャンプ
	if (pInputKey->GetTrigger(DIK_SPACE) == true)
	{
		if (m_bJump != true)
		{
			m_bJump = true;
			m_move.y += PLAYER_JUMP;
			m_pMotion->SetType(CMotion::TYPE_JUMP);
		}
	}

	//角度の正規化
	if (m_rotDest.y - m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_rot += (m_rotDest - m_rot) * 0.5f;

	m_move.y -= GRAVITY; //重力加算

	//動いていたら
	if (m_move.x != 0 || m_move.z != 0)
	{//モーションをMOVEに
		m_pMotion->SetType(CMotion::TYPE_MOVE);
	}

	//前回の位置保存
	m_posOld = m_pos;
	
	//位置更新
	m_pos += m_move;

	//当たり判定
	Collision();

	if (m_bLanding == true)
	{
		m_pos.y = m_posOld.y;
		m_move.y = 0.0f;
		m_bJump = false;
	}

	//起伏
	//CObject3D* obj3d = CManager::GetObj3D();
	//float fHeight = obj3d->GetHeight(m_pos);
	//m_pos.y = fHeight;
	
	if (m_pos.y < 0.0f)
	{
		if (m_bJump == true)
		{
			m_pMotion->SetType(CMotion::TYPE_LANDING);
			m_bJump = false;
		}
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}
	
	//移動量初期化
	m_move.x = 0.0f;
	if (m_bJump == false)
	{
		m_move.y = 0.0f;
	}
	m_move.z = 0.0f;
}
//==================
// アクション処理
//==================
void CPlayer::Action(void)
{
	CInputKey* pInputKey;
	//キー取得
	pInputKey = CManager::GetInputKey();


#ifdef _DEBUG
	//テスト

#endif
}
//================
// ダメージ処理
//================
void CPlayer::Hit(const int nDamage)
{
	if (m_State == STATE_NONE)
	{
		m_nLife -= nDamage;

		if (m_nLife > 0)
		{
			State(STATE_HIT);
		}
		if (m_nLife <= 0)
		{
			m_nLife = 0;
			State(STATE_DEAD);
		}
	}
}
//=================
// 状態遷移処理
//=================
void CPlayer::State(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NONE:
		break;
	case STATE_HIT:
		m_nCntState = 60;
		break;
	case STATE_DEAD:
		m_bUse = false;
		break;

	default:
		break;
	}
}
//====================
// 当たり判定
//====================
bool CPlayer::Collision(void)
{
	CDebugProc* pDegub = CManager::GetDebug();

	//当たり判定のポインタ取得
	CCollision* pColl = CManager::GetColl();

	//if (bColl == true || bCOLL == true)
	//{
	//	pDegub->Print("HIT");

	//	if (m_bLanding == false)
	//	{
	//		m_pos.x = m_posOld.x;
	//		m_pos.z = m_posOld.z;
	//	}
	//}
	//else
	//{
	//	m_bLanding = false;
	//	if (m_pos.y >= 0.0f)
	//	{
	//		m_bJump = true;
	//	}
	//}

	//return bColl;
	return false;
}