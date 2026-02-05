//===============================================
//
// プレイヤー[player.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "sound.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "meshcircle.h"
#include "damegeArea.h"
#include "effect.h"
#include "particle.h"
#include "interact.h"
#include "screen_flash.h"

//静的メンバ変数
bool CPlayer::m_bUse = true;

//===============================================
// コンストラクタ
//===============================================
CPlayer::CPlayer(int nPriority):CObject(nPriority)
{
	//値をクリア
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_move = VECTOR3_NULL;
	m_size = VECTOR3_NULL;
	m_posOld = VECTOR3_NULL;

	m_bUse = true;
	m_bMove = false;
	m_State = STATE_NONE;
	m_nCntState = 0;

	m_nLife = 0;
	m_fSpeed = 0;
	m_nNumModel = 0;
	m_mtxWorld = {};

	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		m_apModel[nCnt] = {};
	}
	m_pMotion = nullptr;

	m_nCntStackt = 0;

	m_BaseColor = { 1.0f,1.0f,1.0f,1.0f };
	m_DamageColor = { 1.0f, 1.0f, 1.0f,0.0f };

	m_pFlowerCnt = nullptr;
	m_bHaveBouquet = false;

	m_bStun = false;
	m_nStunTime = 0;

	m_nStayTime = 0;

	m_pThrowLockIcon = nullptr;
}
//===============================================
// デストラクタ
//===============================================
CPlayer::~CPlayer()
{
}
//===============================================
// 生成処理
//===============================================
CPlayer* CPlayer::Create(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//インスタンス生成
	CPlayer* pPlayer = new CPlayer;
	//初期化
	if (FAILED(pPlayer->Init(pFileName, pos, rot)))
	{
		delete pPlayer;
		return nullptr;
	}
	return pPlayer;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CPlayer::Init(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//位置・向き設定
	m_pos = pos;
	m_rot = rot;
	m_posOffSet = m_pos;

	//モデル生成
	for (int nCnt = 0; nCnt < PLAYER_MODEL; nCnt++)
	{
		// NULLチェック
		if (m_apModel[nCnt] == nullptr)
		{
			// モデルをクリアに
			m_apModel[nCnt] = nullptr;
		}
	}

	//モデル読み込み
	CModel::Load(pFileName, m_apModel, m_nNumModel);
	//モーション読み込み
	m_pMotion = CMotion::Load(pFileName);
	// 最大モデル数分
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// NULLチェック
		if (m_apModel[nCnt] != nullptr)
		{
			// サイズ取得
			m_size = m_apModel[nCnt]->GetSize();
		}
	}
	// 影生成
	m_pShadow = CShadow::Create(m_pos, m_rot, m_size.x * 3.5f, m_size.z * 3.0f, CShadow::TYPE_CIRCLE);

	//if (CManager::GetMode() == CScene::MODE_GAME)
	{
		// ステータスを設定
		m_nLife = PLAYER_LIFE;
		m_fSpeed = PLAYER_SPEED;

		// HPゲージ生成
		if (m_pGaugeM == nullptr)
		{
			m_pGaugeM = CGaugeModel::Create(D3DXVECTOR3(-1500.0f, 50.0f, 1000.0f), (float)m_nLife, D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f));
			m_pGaugeM->CreateFrame("data\\MODEL\\gaugeFrame00.x", D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));	// 枠生成
		}

		// 花カウンター生成
		if (m_pFlowerCnt == nullptr)
		{
			m_pFlowerCnt = CFlowerCnt::Create(D3DXVECTOR3(-1500.0f, -200.0f, 1000.0f), D3DXVECTOR3(200.0f,200.0f,50.0f));
		}

		D3DXVECTOR3 Range = m_pos;	// 投げる範囲可視化用シリンダー用位置
		Range.y += m_size.y * 2.0f;	// 少しあげる
		// 投げる範囲可視化用円柱生成
		m_pThrowRange = CMeshCylinder::Create(Range, rot, PLAYER_RADIUS, m_size.y / 2.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), CMeshCylinder::TYPE_BOTHSIDES);
		// 投げられない時用アイコン生成
		m_pThrowLockIcon = CThrowLockIcon::Create("data\\TEXTURE\\NoThrow.png", m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	SetObjType(TYPE_PLAYER);// オブジェクトのタイプ
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{
	// 使用していない状態にする
	m_bUse = false;

	// モデルの総破棄
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// NULLチェック
		if (m_apModel[nCnt] != nullptr)
		{
			// モデルの終了処理＆破棄
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}
	// NULLチェック
	if (m_pMotion != nullptr)
	{
		//モーション破棄
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// モードがゲームなら
		//if (CManager::GetMode() == CScene::MODE_GAME || CManager::GetMode() == CScene::MODE_GAME)
		{
			// スタンしている
			if (m_bStun == true) { Stun(); }
			// スタンしていない
			else if (m_bStun == false)
			{
				//移動
				Move();

				// OBBを更新
				UpdateOBB();

				//行動
				Action();

				// 花束を持っていれば
				if (m_bHaveBouquet == true)
				{

				}
			}

			// ライフが０
			if (m_nLife <= 0)
			{
				m_bUse = false;	// 使用していない状態にする
			}

			// HPゲージ
			if (m_pGaugeM != nullptr)
			{
				// 色設定用変数（通常色）
				D3DCOLOR col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);

				// 一定量下回っていたら
				if (m_nLife <= PLAYER_LIFE / 1.5f)
				{// 黄色
					col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				}
				if (m_nLife <= PLAYER_LIFE / 4)
				{// 赤色
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}

				// ゲージの設定
				m_pGaugeM->Set((float)m_nLife, col);
			}

			// 投げる範囲可視化用円柱
			if (m_pThrowRange != nullptr)
			{
				D3DXVECTOR3 ThrowRange = m_pos;
				ThrowRange.y += m_size.y * 2.0f;
				// 投げる範囲可視化用円柱の位置設定
				m_pThrowRange->SetPos(ThrowRange);

				// 花束を持っていれば
				if (m_bHaveBouquet == true)
				{
					m_pThrowRange->SetUse(false);	// 見えなくする
				}
				// 持っていない
				else
				{
					m_pThrowRange->SetUse(true);	// 見えるようにする
				}
			}

			// 投げられない時用アイコン
			if (m_pThrowLockIcon != nullptr)
			{
				// 位置追従
				m_pThrowLockIcon->SetPos(D3DXVECTOR3(m_pos.x + 125.0f, m_pos.y + 250.0f, m_pos.z));
			}

			// ヒットしていれば
			if (m_State == STATE_HIT)
			{
				//Blink();
			}
			// 状態カウンターを減らす
			m_nCntState--;
			// カウンターが０以下なら
			if (m_nCntState <= 0)
			{
				// スタンしていれば
				if (m_bStun == true)
				{
					m_bStun = false;	// スタンしていない状態にする
				}

				m_nCntState = 0;	// カウンターを０にする
				// 状態を元に戻す
				State(STATE_NONE);
			}
		}
		// NULLチェック
		if (m_pMotion != nullptr)
		{
			//モーション更新
			m_pMotion->Update(m_nNumModel, m_apModel);
		}
		// NULLチェック
		if (m_pShadow != nullptr)
		{
			// 影の位置更新
			m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 0.3f, m_pos.z));
			m_pShadow->SetRot(m_rot);
		}
	}
	// 使用していない状態なら
	else if(m_bUse == false)
	{
		// ゲームを終わる
		CGame::SetMode(CGame::MODE_FIN);
	}
}
//===============================================
// 描画処理
//===============================================
void CPlayer::Draw(void)
{
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("プレイヤーの位置：%.1f,%.1f,%.1f", m_pos.x, m_pos.y, m_pos.z);
	pDegub->Print("プレイヤーの角度：%.1f,%.1f,%.1f", m_rot.x, m_rot.y, m_rot.z);
	pDegub->Print("プレイヤーの移動量：%.1f,%.1f,%.1f", m_move.x, m_move.y, m_move.z);
	pDegub->Print("プレイヤーのサイズ：%.1f,%.1f,%.1f", m_size.x, m_size.y, m_size.z);
	pDegub->Print("プレイヤーのHP：%d", m_nLife);
	pDegub->Print("プレイヤーのスピード：%f", m_fSpeed);
	if (m_State == STATE_HIT)
	{
		pDegub->Print("!!!!!ダメージヒット!!!!!");
	}	
	if (m_bStun == true)
	{
		pDegub->Print("スタン!!!!!!");
	}
	pDegub->Print("ゴミ：%d / 雑貨：%d / 花：%d", CInteract::GetNumDust(), CInteract::GetNumGoods(), CInteract::GetNumFlower());


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
		// NULLチェック
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Draw();	// モデルを描画
		}
	}
}
//===============================================
// 移動処理
//===============================================
void CPlayer::Move(void)
{
	// キー情報取得
	CInputKey* pInputKey = CManager::GetInputKey();
	// パッド情報取得
	CInputPad* pInputPad = CManager::GetInputPad();
	// カメラの情報取得
	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 CameraRot = pCamera->GetRot();	// カメラの角度取得

	//-------------------------------------------
	// キー移動
	//------------------------------------------- 
	// 左
	if (pInputKey->GetPress(DIK_A) == true)
	{
		// 左上
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI * 0.75f;
		}
		// 左下
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI * 0.25f;
		}
		// 左
		else
		{
			m_move.x += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y + D3DX_PI / 2;
		}
	}
	// 右
	if (pInputKey->GetPress(DIK_D) == true)
	{
		// 右上
		if (pInputKey->GetPress(DIK_W) == true)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI * 0.75f) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI * 0.75f;
		}
		// 右下
		else if (pInputKey->GetPress(DIK_S) == true)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI * 0.25f) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI * 0.25f;
		}
		// 右
		else
		{
			m_move.x -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI / 2;
		}
	}
	// 前
	if (pInputKey->GetPress(DIK_W) == true)
	{
		if (pInputKey->GetPress(DIK_A) == false && pInputKey->GetPress(DIK_D) == false)
		{
			m_move.x -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y - D3DX_PI;
		}
	}
	// 後
	if (pInputKey->GetPress(DIK_S) == true)
	{
		if (pInputKey->GetPress(DIK_A) == false && pInputKey->GetPress(DIK_D) == false)
		{
			m_move.x += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
			m_move.z += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;

			m_rotDest.y = CameraRot.y;
		}
	}

	//-------------------------------------------
	// パッド移動
	//-------------------------------------------
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
			float angle = atan2f(fLX, fLY);  // スティックの角度
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

	//角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 角度を更新
	m_rot += (m_rotDest - m_rot) * 0.5f;

	m_move.y -= GRAVITY; //重力加算

	//動いていたら
	if (m_move.x != 0 || m_move.z != 0)
	{//モーションをMOVEに
		if (m_pMotion->GetType() != MOTION_MOVE && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_MOVE);
		}
		m_bMove = true;		// 移動していることにする
		m_nStayTime = 0;	// 止まった時間を０に
	}
	// 動いていないなら
	else
	{
		// モーションをニュートラルに
		if (m_pMotion->GetType() != MOTION_NEUTRAL && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_NEUTRAL);
		}
		m_bMove = false;	// 移動していないことにする
		m_nStayTime++;		// 止まった時間を加算
	}
	//前回の位置保存
	m_posOld = m_pos;
	
	//位置更新
	m_pos += m_move;

	//当たり判定
	bool bColl = Collision();
	CheckStack(bColl, m_bMove);
	
	// フィールドの情報取得
	D3DXVECTOR3 FieldPos = CGame::GetField()->GetPos();
	float fFieldHeight = CGame::GetField()->GetHeight();
	float fFieldWidth = CGame::GetField()->GetWidth();

	// X軸移動制限
	if (m_pos.x <= FieldPos.x - fFieldWidth || m_pos.x >= FieldPos.x + fFieldWidth)
	{
		m_pos.x = m_posOld.x;
	}
	// Z軸移動制限
	if (m_pos.z <= FieldPos.z - fFieldHeight || m_pos.z >= FieldPos.z + fFieldHeight)
	{
		m_pos.z = m_posOld.z;
	}

	// 今回の移動量を保存
	m_moveOld = m_move;

	// Yが０より下になったら以下
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}
	
	//移動量初期化
	m_move.x = 0.0f;
	m_move.z = 0.0f;

	m_fSpeed = PLAYER_SPEED;	// スピードを元に戻す
}
//===============================================
// アクション処理
//===============================================
void CPlayer::Action(void)
{
	//キー情報取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド情報取得
	CInputPad* pInputPad = CManager::GetInputPad();
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();

	// 投げる
	if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(0) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_A) == true)
	{
		// 投げる
		Throw();
	}

	// デバッグキー
#ifdef _DEBUG

	if (pInputKey->GetTrigger(DIK_1) == true)
	{
		CDamegeArea::Create(m_pos, m_rot, 100.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.25f), CDamegeArea::TYPE_NONE, 1, 60);
	}
	if (pInputKey->GetTrigger(DIK_2) == true)
	{
		CInteract::Create(D3DXVECTOR3(-100.0f, 500.0f, 100.0f), VECTOR3_NULL, CInteract::TYPE_DUST);
		CInteract::Create(D3DXVECTOR3(100.0f, 500.0f, 100.0f), VECTOR3_NULL, CInteract::TYPE_GOODS);
		CInteract::Create(D3DXVECTOR3(-100.0f, 500.0f, -100.0f), VECTOR3_NULL, CInteract::TYPE_DUST);
		CInteract::Create(D3DXVECTOR3(100.0f, 500.0f, -100.0f), VECTOR3_NULL, CInteract::TYPE_GOODS);
	}
	if (pInputKey->GetTrigger(DIK_3) == true)
	{
		CInteract::Create(D3DXVECTOR3(100.0f, 500.0f, -100.0f), VECTOR3_NULL, CInteract::TYPE_FLOWER);
	}
	if (pInputKey->GetTrigger(DIK_4) == true)
	{
		//CDamegeArea::Create(m_pos, m_rot, 75.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.25f), CDamegeArea::TYPE_THUNDER, 1, 120);
		CParticle::Create(D3DXVECTOR3(0.0f, 200.0f, 0.0f), VECTOR3_NULL, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 30, 20, CParticle::TYPE_SPRAY);
		//CParticle::SetTargetPos(m_pos);
	}
	if (pInputKey->GetTrigger(DIK_5) == true)
	{
		SetStun(60);
	}
	if (pInputKey->GetTrigger(DIK_6) == true)
	{
		// インタラクトオブジェクトランダム大量配置
		const int gridCountX = 10;		// X方向の数
		const int gridCountZ = 10;		// Z方向の数
		const float spacing = 60.0f;	// 配置間隔

		for (int x = -gridCountX / 2; x < gridCountX / 2; ++x)
		{
			for (int z = -gridCountZ / 2; z < gridCountZ / 2; ++z)
			{
				D3DXVECTOR3 pos(
					x * spacing,
					500.0f,
					z * spacing
				);

				// タイプをランダムで選択
				const int Max_Probability = 20;
				int nType = rand() % (Max_Probability + 1);
				CInteract::TYPE type;

				// ゴミ
				if (nType >= 0 && nType < Max_Probability / 2)
				{
					type = CInteract::TYPE_DUST;
				}
				// 雑貨
				else if (nType >= Max_Probability / 2 && nType < Max_Probability)
				{
					type = CInteract::TYPE_GOODS;
				}
				// 花
				else
				{
					type = CInteract::TYPE_FLOWER;
				}
				//CObjectPhysics::Create("data\\MODEL\\someBooks00.x", pos, D3DXVECTOR3(0, 0, 0));

				CInteract::Create(pos, D3DXVECTOR3(0, 0, 0), type);
			}
		}
	}
	if (pInputKey->GetTrigger(DIK_9) == true)
	{
		CGame::GetBoss()->Hit((int)(PLAYER_LIFE));
	}
	if (pInputKey->GetPress(DIK_0) == true)
	{
		Hit(1);
	}
	if (pInputKey->GetTrigger(DIK_L) == true)
	{
		CSound* pSound = CManager::GetSound();
		pSound->Stop();
	}
	if (pInputKey->GetTrigger(DIK_T) == true)
	{
		CGame::GetTutorial()->SetClear(true);
	}
#endif
}
//===============================================
// 投げる
//===============================================
void CPlayer::Throw(void)
{
	// 花束を持っていないなら
	if (m_bHaveBouquet != true)
	{
		// ステージモデルのみ調べる
		CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();		// 次のオブジェクトを取得
			CObject::TYPE type = pObj->GetObjType();	// 種類取得

			// 種類が一致していれば
			if (type == CObject::TYPE_INTERACT)
			{
				// 物理オブジェクト情報取得
				CInteract* pObjPhysics = (CInteract*)pObj;
				D3DXVECTOR3 PhysicsPos = pObjPhysics->GetPos();		// 位置
				D3DXVECTOR3 PhysicsSize = pObjPhysics->GetSize();	// サイズ
				// 半径をサイズから算出
				float fRadius = max(PhysicsSize.x, max(PhysicsSize.y, PhysicsSize.z)) * 0.5f;

				// 半径での判定
				if (CManager::GetColl()->Radius(PhysicsPos, fRadius, m_pos, PLAYER_RADIUS) == true)
				{// 範囲内なら

					//------------------------------------------
					// プレイヤーの向いている方向に飛ばす
					//------------------------------------------
					static const float fPower = 5.0f;		// 最大吹き飛ばし力
					static const float fPowerY = 10.0f;		// 最大吹き飛ばし力

					// 移動量
					D3DXVECTOR3 move = VECTOR3_NULL;
					move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * fPower, 0.0f, cosf(m_rot.y - D3DX_PI) * fPower);

					// 上方向
					move.y += fPowerY * 0.3;

					// 吹き飛ばし
					pObjPhysics->SetBlow(true, move);

					// 投げのチュートリアルなら
					if (CGame::GetType() == CGame::TYPE_TUTORIAL && CGame::GetTutorial()->GetMode() == CTutorial::MODE_THROW)
					{
						// チュートリアルをクリア判定にする
						CGame::GetTutorial()->SetClear(true);
					}
				}
			}
			// 次のオブジェクトへ
			pObj = pObjNext;
		}
		// 音
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_THROWP);
	}
	else
	{
		// アイコンを使用
		m_pThrowLockIcon->SetUse(true);
	}
}
//===============================================
// ダメージ処理
//===============================================
void CPlayer::Hit(const int nDamage)
{
	// 状態が通常時なら
	if (m_State == STATE_NONE)
	{
		// ダメージを受ける
		m_nLife -= nDamage;

		// HPがまだあれば
		if (m_nLife > 0)
		{
			State(STATE_HIT);	// Hit状態にする
		}
		// HPがなければ
		else if (m_nLife <= 0)
		{
			m_nLife = 0;
			State(STATE_DEAD);	// 死亡状態にする
		}

		// 各種振動処理
		m_pGaugeM->SetShakeInfo(true, 30, D3DXVECTOR3(25.0f, 25.0f, 25.0f));					// ゲージ
		m_pFlowerCnt->SetShakeInfo(true, 30, D3DXVECTOR3(25.0f, 25.0f, 25.0f));					// 花カウンター
		CGame::GetBoss()->GetGauge()->SetShakeInfo(true, 30, D3DXVECTOR3(25.0f, 25.0f, 25.0f));	// ボスゲージ
		CStage::ShakeAll();																		// ステージモデル

		// 画面フラッシュ演出
		CScreenFlash::Create(nullptr, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

		// カメラを振動させる
		CManager::GetCamera()->SetShake(20.0f, 20.0f, 30);

		// パッド情報取得
		CInputPad* pInputPad = CManager::GetInputPad();
		pInputPad->SetVibration(20000, 20000, 0, 30);	// パッドを振動させる

		// 音
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_HIT);
	}
}
//===============================================
// 点滅処理
//===============================================
void CPlayer::Blink(void)
{
	const int nBlinkFrame = 5;	// 点滅フレーム数
	
	if ((m_nCntState / nBlinkFrame) % 2 == 0)
	{// ダメージ色にする
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetColor(m_DamageColor);
			}
		}
	}
	else
	{// 通常色に戻す
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetColor(m_BaseColor);
			}
		}
	}
}
//===============================================
// 状態遷移処理
//===============================================
void CPlayer::State(STATE state)
{
	m_State = state;

	switch (m_State)
	{
		//-----------------------------------
		// 通常
		//-----------------------------------
	case STATE_NONE:
		// 色を戻す
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetColor(m_BaseColor);
			}
		}
		break;

		//-----------------------------------
		// ヒット
		//-----------------------------------
	case STATE_HIT:
		m_nCntState = 60;
		break;

		//-----------------------------------
		// 死亡
		//-----------------------------------
	case STATE_DEAD:
		m_nLife = 0;
		m_pGaugeM->SetDraw(false);
		break;
		
		//-----------------------------------
		// スタン
		//-----------------------------------
	case STATE_STUN:
		m_nCntState = m_nStunTime;
		m_bStun = true;
		break;
	default:
		break;
	}
}
//===============================================
// スタンしている状態
//===============================================
void CPlayer::Stun(void)
{

}
//===============================================
// 当たり判定
//===============================================
bool CPlayer::Collision(void)
{
	bool hit = false;

	return hit;
}
//===============================================
// OBBの更新
//===============================================
void CPlayer::UpdateOBB(void)
{
	// 中心座標を現在の位置に
	m_obb.centerPos = m_pos;

	// モデルの回転（Yaw, Pitch, Roll）を行列に
	D3DXMATRIX mRot;
	D3DXMatrixRotationYawPitchRoll(&mRot, m_rot.y, m_rot.x, m_rot.z);

	// 各軸ベクトル（ローカル→ワールド変換）
	m_obb.axis[0] = D3DXVECTOR3(mRot._11, mRot._12, mRot._13); // X軸
	m_obb.axis[1] = D3DXVECTOR3(mRot._21, mRot._22, mRot._23); // Y軸
	m_obb.axis[2] = D3DXVECTOR3(mRot._31, mRot._32, mRot._33); // Z軸

	// 半サイズ設定
	m_obb.size = m_size;
}

//===============================================
// スタックチェック処理
//===============================================
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
//===============================================
// スタック救出処理
//===============================================
void CPlayer::RecoverFromStuck(void)
{
	// デバッグログ出力
	CDebugProc* pDebug = CManager::GetDebug();
	pDebug->Print("スタック検出：リカバリ実行");

	// 強制移動先
	m_pos = m_posOffSet;
	m_move = D3DXVECTOR3(0, 0, 0);
}