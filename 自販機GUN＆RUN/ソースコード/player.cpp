//==============================
//
// プレイヤー[player.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "score_manager.h"
#include "enemy.h"
#include "debugproc.h"
#include "camera.h"
#include "sound.h"
#include "object3D.h"
#include "enemy_base.h"
#include "objectBillboard.h"
#include "collision.h"
#include "particle.h"
#include "collision.h"
#include "vender.h"
#include "sold_out.h"

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
	m_Bullet = CBullet::TYPE_CAN;

	m_bUse = true;
	m_State = STATE_NONE;
	m_nCntState = 0;

	m_nLife = 0;
	m_fSpeed = 0;
	m_nCntContents = 0;
	m_nNumModel = 0;
	m_mtxWorld = {};

	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		m_apModel[nCnt] = {};
	}
	m_pMotion = nullptr;

	m_pShadow = NULL;

	m_nCntStackt = 0;

	m_BaseColor = { 1.0f,1.0f,1.0f,1.0f };
	m_DamageColor = { 1.0f, 1.0f, 1.0f,0.0f };

	m_bCanRestock = false;
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
CPlayer* CPlayer::Create(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer;
	//インスタンス生成
	pPlayer = new CPlayer;
	//初期化
	if (FAILED(pPlayer->Init(pFileName, pos, rot)))
	{
		delete pPlayer;
		return nullptr;
	}
	return pPlayer;
}
//===============
// 初期化処理
//===============
HRESULT CPlayer::Init(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//位置・向き設定
	m_pos = pos;
	m_rot = rot;
	m_posOffSet = m_pos;

	//モデル生成
	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] == NULL)
		{
			m_apModel[nCnt] = nullptr;
		}
	}

	//モデル読み込み
	CModel::Load(pFileName, m_apModel, m_nNumModel);
	//モーション読み込み
	m_pMotion = CMotion::Load(pFileName);
	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		m_size = m_apModel[nCnt]->SetSize();
		m_apModel[nCnt]->SetColorChange(true);
	}

	m_pShadow = CShadow::Create(m_pos, m_rot, m_size.x, m_size.z, CShadow::TYPE_CIRCLE);

	//m_pShadowS = CShadowS::Create(m_pos, m_rot, fRadius);

	m_nLife = PLAYER_LIFE;
	m_fSpeed = PLAYER_SPEED;
	m_nCntContents = 3;
	m_fShotTimer = 0.0f;

	m_BulletCnt = CBullerCntSystem::Create();

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

	//影破棄
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}
	if (m_pShadowS != nullptr)
	{
		m_pShadowS->Uninit();
		delete m_pShadowS;
		m_pShadowS = nullptr;
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
		//移動
		Move();

		//行動
		Action();
		
		if (m_fSpeed > PLAYER_SPEED)
		{
			CEffect::Create(m_pos, m_rot, m_move, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 1, 15);
		}

		if (m_nLife <= 0)
		{
			m_bUse = false;
		}

		if (m_State == STATE_HIT)
		{
			Blink();
		}
		m_nCntState--;
		if (m_nCntState <= 0)
		{
			m_nCntState = 0;
			State(STATE_NONE);
		}

		m_BulletCnt->GetUseCnt()->SetCnt(m_nCntContents);
		m_BulletCnt->GetUseCnt()->SetType(m_Bullet);

		//モーション更新
		if (m_pMotion != nullptr)
		{
			m_pMotion->Update(m_nNumModel, m_apModel);
		}
		if (m_pShadow != nullptr)
		{
			m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 0.3f, m_pos.z));
			m_pShadow->SetRot(m_rot);
		}
	}
	else if(m_bUse == false)
	{
		CGame::SetMode(CGame::MODE_FIN);
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
	pDegub->Print("プレイヤーの移動量：%.1f,%.1f,%.1f", m_move.x, m_move.y, m_move.z);
	pDegub->Print("プレイヤーのサイズ：%.1f,%.1f,%.1f", m_size.x, m_size.y, m_size.z);
	pDegub->Print("プレイヤーのHP：%d", m_nLife);
	pDegub->Print("プレイヤーの中身：%d", m_nCntContents);
	if (m_State == STATE_HIT)
	{
		pDegub->Print("ダメージヒット");
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
	
	if (m_pShadowS != nullptr)
	{
		m_pShadowS->Set(m_pos, m_rot);
		m_pShadowS->Draw();
	}
}
//===================
// 中身を空にする
//===================
void CPlayer::ClearContents(void)
{
	// 中身を１０以上なら
	int nContents = m_nCntContents;

	if (m_nCntContents >= 10)
	{
		nContents = 10;
	}
	float rot = D3DX_PI / (nContents / 2);
	for (int nCnt = 0; nCnt < nContents; nCnt++)
	{
		CBullet* pBullet = nullptr;
		pBullet = CBullet::Create(m_pos, { m_rot.x,m_rot.y + (rot * nCnt),m_rot.z }, CBullet::USER_PLAYER);
		pBullet->SetSkill(true);
	}
	
	// 中身を０に
	m_nCntContents = 0;

	//チュートリアルクリア判定
	if (CGame::GetMode() == CGame::MODE_TUTORIAL)
	{
		CTutorial* pTutorial = CGame::GetTutorial();
		if (pTutorial != nullptr)
		{
			if (pTutorial->GetType() == CTutorial::TYPE_CONTENTS)
			{
				CGame::GetTutorial()->SetClear(true);
			}
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
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

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

	if (pInputPad != nullptr)
	{
		// スティック入力による移動
		const float DEADZONE = 10000.0f;  // デッドゾーン
		XINPUT_STATE* pJoyState = pInputPad->GetJoyStickAngle();
		SHORT lx = pJoyState->Gamepad.sThumbLX;
		SHORT ly = pJoyState->Gamepad.sThumbLY;

		float fLX = (abs(lx) > DEADZONE) ? lx / 32768.0f : 0.0f;
		float fLY = (abs(ly) > DEADZONE) ? ly / 32768.0f : 0.0f;

		if (fLX != 0.0f || fLY != 0.0f)
		{
			// カメラの回転に基づいたスティック方向をワールド方向に変換
			float angle = atan2f(fLX, fLY);  // スティックの角度（ラジアン）
			float rotY = CameraRot.y + angle;

			m_move.x += sinf(rotY) * m_fSpeed;
			m_move.z += cosf(rotY) * m_fSpeed;

			m_rotDest.y = rotY + D3DX_PI;
		}

		const float sensitivity = 0.05f; // 感度調整用
		float rx = pInputPad->GetRightStickX(); // 横
		// デッドゾーン処理
		const float deadZone = 0.2f;
		if (fabsf(rx) > deadZone)
		{
			m_rot.y += rx * sensitivity;
		}
	}

	//マウスの左右移動で視点旋回
	if (pInputMouse != nullptr)
	{
		int mouseMoveX = pInputMouse->GetRelX();

		const float sensitivity = 0.0009f; // 感度調整用

		m_rotDest.y += mouseMoveX * sensitivity;
	}
	if (pInputPad != nullptr)
	{
		const float sensitivity = 0.1f; // 感度調整用

		float rx = pInputPad->GetRightStickX(); // 横

		// デッドゾーン処理
		const float deadZone = 0.2f;
		if (fabsf(rx) > deadZone)
		{
			m_rotDest.y += rx * sensitivity;
		}
	}

	//角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	m_rot += (m_rotDest - m_rot) * 0.5f;

	m_move.y -= GRAVITY; //重力加算

	//動いていたら
	if (m_move.x != 0 || m_move.z != 0)
	{//モーションをMOVEに
		if (m_pMotion->GetType() != CMotion::TYPE_MOVE)
		{
			m_pMotion->SetType(CMotion::TYPE_MOVE);
		}
		//m_pMotion->SetType(CMotion::TYPE_MOVE);
		m_bMove = true;
	}
	else
	{
		if (m_pMotion->GetType() != CMotion::TYPE_NEUTRAL)
		{
			m_pMotion->SetType(CMotion::TYPE_NEUTRAL);
		}
		m_bMove = false;
	}
	//前回の位置保存
	m_posOld = m_pos;
	
	//位置更新
	m_pos += m_move;

	//当たり判定
	bool bColl = Collision();
	CheckStack(bColl, m_bMove);
	
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}
	
	//移動量初期化
	m_move.x = 0.0f;
	m_move.z = 0.0f;
}
//==================
// アクション処理
//==================
void CPlayer::Action(void)
{
	//キー取得
	 CInputKey* pInputKey = CManager::GetInputKey();
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	CSound* pSound = CManager::GetSound();

	CRestock* pRestock = CVender::GetpRestock();
	//弾発射
	if (m_bCanRestock != true && pRestock->GetUse() != true)
	{
		if (pInputMouse->GetPress(1) == false)
		{
			if (pInputMouse->GetPress(0) == true || pInputPad->GetR2Press(30) == true)
			{
				if (m_nCntContents > 0)
				{//中身あり
					m_fShotTimer -= SHOT_TIMESUB;
					if (m_fShotTimer <= 0.0f)
					{
						CBullet::Create(m_pos, m_rot, CBullet::USER_PLAYER);
						m_nCntContents--;

						pSound->PlaySound(CSound::SOUND_LABEL_SHOT);

						m_fShotTimer = SHOT_INTERVAL;

						//チュートリアルクリア判定
						if (CGame::GetMode() == CGame::MODE_TUTORIAL)
						{
							CTutorial* pTutorial = CGame::GetTutorial();
							if (pTutorial != nullptr)
							{
								if (pTutorial->GetType() == CTutorial::TYPE_BULLET)
								{
									CGame::GetTutorial()->SetClear(true);
								}
							}
						}
					}
				}
				else
				{
					m_BulletCnt->SetBlink(true);

					if (CSoldOut::GetUse() == false)
					{
						CSoldOut::Create();
					}

					pSound->PlaySound(CSound::SOUND_LABEL_MISS);
				}
			}
			else
			{
				m_fShotTimer = 0.0f;
			}
		}
	}

	ChangeBullet();

#ifdef _DEBUG
	//パーティクル
	if (pInputKey->GetTrigger(DIK_2) == true)
	{
		CParticle::Create({ m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z }, m_rot, D3DXCOLOR(1.0f, 0.8f, 0.2f, 1.0f), 1, 8.0f, CParticle::TYPE_SPRAY);
	}

	if (pInputKey->GetTrigger(DIK_3) == true)
	{
		CEffectCircle::Create(m_pos, m_rot, 30.0f, D3DXCOLOR(0.8f, 0.5f, 0.0f, 0.3f), CMeshCircle::TYPE_BOTHSIDES, CEffectCircle::TYPE_UPDOWN);
	}

	if (pInputKey->GetTrigger(DIK_0) == true)
	{
		m_nLife -= 10;
	}
	CScoreMana* pScore = CGame::GetBreakCnt();
	CScoreMana* pScore1= CGame::GetTotalScore();

	if (pInputKey->GetTrigger(DIK_9) == true)
	{
		pScore->AddScore(1);
	}
	if (pInputKey->GetTrigger(DIK_8) == true)
	{
		pScore1->AddScore(1);	
	}
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("破壊数：%d", pScore->GetScore());
	pDegub->Print("トータル：%d", pScore1->GetScore());

#endif
}
//
//
//
void CPlayer::ChangeBullet(void)
{
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	if (pInputMouse->GetPress(0) == false)
	{
		if (pInputMouse->GetTrigger(1) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_R1) == true)
		{
			m_BulletCnt->Change();

			CSound* pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_CHANGE);
		}
	}
}
//================
// ダメージ処理
//================
void CPlayer::Hit(const int nDamage)
{
	CSound* pSound = CManager::GetSound();

	if (m_State == STATE_NONE)
	{
		m_nLife -= nDamage;

		CGame::GetFlash()->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

		if (m_nLife > 0)
		{
			State(STATE_HIT);
		}
		if (m_nLife <= 0)
		{
			m_nLife = 0;
			State(STATE_DEAD);
		}

		CManager::GetCamera()->SetShake(5.0f, 5.0f, 30);

		pSound->PlaySound(CSound::SOUND_LABEL_HIT);

		//パッド
		CInputPad* pInputPad = CManager::GetInputPad();
		pInputPad->SetVibration(20000, 20000, 0, 30);

	}
}
//===============
// 点滅処理
//===============
void CPlayer::Blink(void)
{
	const int nBlinkFrame = 5;//点滅フレーム数

	if ((m_nCntState / nBlinkFrame) % 2 == 0)
	{//ダメージ色にする
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != NULL)
			{
				m_apModel[nCnt]->SetColor(m_DamageColor);
			}
		}
	}
	else
	{//通常色に戻す
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != NULL)
			{
				m_apModel[nCnt]->SetColor(m_BaseColor);
			}
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
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != NULL)
			{
				m_apModel[nCnt]->SetColor(m_BaseColor);
			}
		}
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
	CDebugProc* pDebug = CManager::GetDebug();
	CCollision* pColl = CGame::GetColl();

	bool hit = false;
	const int maxIterations = 3;
	D3DXVECTOR3 moveVec = m_pos - m_posOld;

	// 自分のOBB軸を生成
	D3DXVECTOR3 axes0[3];
	{
		D3DXMATRIX rotMtx;
		D3DXMatrixRotationYawPitchRoll(&rotMtx, m_rot.y, m_rot.x, m_rot.z);
		axes0[0] = D3DXVECTOR3(rotMtx._11, rotMtx._12, rotMtx._13);
		axes0[1] = D3DXVECTOR3(rotMtx._21, rotMtx._22, rotMtx._23);
		axes0[2] = D3DXVECTOR3(rotMtx._31, rotMtx._32, rotMtx._33);
		for (int i = 0; i < 3; ++i)
		{
			D3DXVec3Normalize(&axes0[i], &axes0[i]);
		}
	}

	for (int iter = 0; iter < maxIterations; ++iter)
	{
		bool anyHit = false;

		// 複数対象に対応するため、毎回すべてチェック
		std::vector<D3DXVECTOR3> contactNormals;

		D3DXVECTOR3 normal;
		if (pColl->ToEnemy(m_pos, m_rot, m_size, TYPE_PLAYER, normal))
		{
			contactNormals.push_back(normal);
			anyHit = true;
			hit = true;
		}
		if (pColl->ToVender(m_pos, m_rot, m_size, TYPE_PLAYER, normal))
		{
			contactNormals.push_back(normal);
			anyHit = true;
			hit = true;
		}

		if (pColl->ToStage(m_pos, m_rot, m_size, normal,TYPE_PLAYER)) 
		{
			contactNormals.push_back(normal);
			anyHit = true;
			hit = true;
		}

		if (!anyHit)
		{
			break;
		}

		D3DXVECTOR3 totalNormal(0, 0, 0);
		for (const auto& n : contactNormals) 
		{
			totalNormal += n;
		}
		if (D3DXVec3LengthSq(&totalNormal) > 0.0001f) 
		{
			D3DXVec3Normalize(&totalNormal, &totalNormal);

			float penetration = 0.0f;
			pColl->OverlapOnAxis(
				moveVec, totalNormal,
				axes0, m_size,
				axes0, m_size,
				penetration
			);
			if (penetration > 0.001f)
			{
				m_pos += totalNormal * penetration;
			}

			// スライドベクトルを計算
			moveVec = m_pos - m_posOld;
			float dot = D3DXVec3Dot(&moveVec, &totalNormal);
			moveVec = moveVec - totalNormal * dot;
			m_pos = m_posOld + moveVec;
		}
	}

	pColl->ToGimmick(m_pos, m_size, TYPE_PLAYER);

	if (hit) pDebug->Print("HIT");
	return hit;
}

//========================
// スタックチェック処理
//========================
void CPlayer::CheckStack(bool bColl, bool bMove)
{
	if (bColl == true && bMove == false)
	{
		m_nCntStackt++;

		// 一定フレーム以上スタックしていたら救出
		if (m_nCntStackt >= 180)
		{
			RecoverFromStuck();
			m_nCntStackt = 0;
		}
	}
	else
	{
		m_nCntStackt = 0;
	}
}
//======================
// スタック救出処理
//======================
void CPlayer::RecoverFromStuck(void)
{
	// デバッグログ出力
	CDebugProc* pDebug = CManager::GetDebug();
	pDebug->Print("スタック検出：リカバリ実行");

	// 強制移動先
	m_pos = m_posOffSet;
	m_move = D3DXVECTOR3(0, 0, 0);

	// 影の位置も更新
	if (m_pShadow)
	{
		m_pShadow->SetPos(m_pos);
	}

	CScoreMana* pTotalScore = CGame::GetTotalScore();
	pTotalScore->AddScore(1);
}