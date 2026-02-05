//===============================================
//
// ボス[boss.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "boss.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "debugproc.h"
#include "flowerVase.h"
#include "particle.h"
#include "file.h"

// 静的メンバ変数
int CBoss::m_nMoveTarget = 0;
int CBoss::m_nCntAttack = 0;
CBossAttack* CBoss::m_pAttack = nullptr;
CBossAI* CBoss::m_pAI = nullptr;

//===============================================
// コンストラクタ
//===============================================
CBoss::CBoss(int nPriority) :CObject(nPriority)
{
	// 値をクリア
	m_mtxWorld = {};
	m_pos = VECTOR3_NULL;
	m_posOld = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_rotDest = VECTOR3_NULL;
	m_move = VECTOR3_NULL;
	m_size = VECTOR3_NULL;

	for (int nCnt = 0; nCnt < BOSS_MODEL; nCnt++)
	{
		m_apModel[nCnt] = {};
	}
	m_pMotion = nullptr;
	m_nNumModel = 0;

	m_pGaugeM = nullptr;
	m_nLife = 0;
	m_nCntState = 0;
	m_bUse = true;
	m_bAction = false;
	m_bAttackCancel = false;
	m_State = STATE_NONE;

	m_nMoveTarget = 0;	// 移動目標を０にする
}
//===============================================
// デストラクタ
//===============================================
CBoss::~CBoss()
{

}
//===============================================
// 生成処理
//===============================================
CBoss* CBoss::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// インスタンス生成
	CBoss* pBoss = new CBoss;

	// 初期化
	if (FAILED(pBoss->Init(pos, rot)))
	{// 失敗したら
		delete pBoss;	// インスタンス破棄
		return nullptr;	// NULLを返す
	}

	// 値を返す
	return pBoss;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CBoss::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// 値を代入
	m_pos = pos;
	m_rot = rot;
	// 値を初期化
	m_nLife = BOSS_LIFE;
	m_bUse = true;
	m_bAction = false;
	m_bAttackCancel = false;
	m_State = STATE_MOVE;

	// 読み込むファイル名
	const char* pFilename = "data\\MOTION\\motionBoss.txt";

	//モデル読み込み
	CModel::Load(pFilename, m_apModel, m_nNumModel);
	//モーション読み込み
	m_pMotion = CMotion::Load(pFilename);
	// サイズ設定
	for (int nCnt = 0; nCnt < BOSS_MODEL; nCnt++)
	{
		m_size = m_apModel[nCnt]->GetSize();
	}
	
	// ゲームなら
	if (CGame::GetType() == CGame::TYPE_GAME)
	{
		// ゲージ生成
		if (m_pGaugeM == nullptr)
		{
			m_pGaugeM = CGaugeModel::Create(D3DXVECTOR3(250.0f, 0.0f, 1000.0f), (float)m_nLife, D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f), D3DXVECTOR3(52.5f, 150.0f, 150.0f));
			m_pGaugeM->CreateFrame("data\\MODEL\\gaugeFrame00.x", D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));	// フレーム生成
			m_pGaugeM->CreateBack("data\\MODEL\\gauge00.x", D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));			// 背景生成
		}

		// HPを少し減らす
		m_nLife = (int)(m_nLife / 1.5f);

		// 攻撃生成
		if (m_pAttack == nullptr)
		{
			m_pAttack = new CBossAttack;
		}
		// AI生成
		if (m_pAI == nullptr)
		{
			m_pAI = new CBossAI;
		}
	}
	//オブジェクトの種類設定
	SetObjType(TYPE_BOSS);

	// 値を返す
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CBoss::Uninit(void)
{
	// モデル破棄
	for (int nCnt = 0; nCnt < BOSS_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();	// 終了処理
			delete m_apModel[nCnt];		// 破棄
			m_apModel[nCnt] = nullptr;		// NULLにする
		}
	}
	//モーション破棄
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;		// 破棄
		m_pMotion = nullptr;	// NULLにする
	}
	// 攻撃破棄
	if (m_pAttack != nullptr)
	{
		delete m_pAttack;		// 破棄
		m_pAttack = nullptr;	// NULLにする
	}
	// AI破棄
	if (m_pAI != nullptr)
	{
		delete m_pAI;			// 破棄
		m_pAI = nullptr;		// NULLにする
	}

	// オブジェクトの破棄
	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CBoss::Update(void)
{
	// 使用しているなら
	if (m_bUse == true)
	{
		// NULLチェック
		if (m_pAI != nullptr)
		{
			// AIの更新
			m_pAI->Update();
		}

		// 花瓶を調べる
		ChekFlowerVase();

		// 攻撃しないなら
		if (m_bAction == false)
		{
			// カウンターを減らす
			m_nCntState--;

			// カウンターが０以下なら
			if (m_nCntState <= 0)
			{
				// 通常なら
				if (m_State == STATE_NONE)
				{
					// 移動状態にする
					State(STATE_MOVE);
				}
				// 移動以外なら
				else if (m_State != STATE_MOVE)
				{
					// 通常にする
					State(STATE_NONE);
				}
			}
		}
		// 攻撃するなら
		else if(m_bAction == true)
		{
			// 攻撃キャンセルしないなら
			if (m_bAttackCancel != true)
			{
				// 攻撃
				m_State = STATE_ACTION;
			}
		}

		// 状態ごとの処理
		switch (m_State)
		{
		case STATE_MOVE:
			//移動
			Move();
			break;
		case STATE_ACTION:
			// 攻撃
			Action();
			break;
		default:
			break;
		}

		// ゲージの設定
		if (m_pGaugeM != nullptr)
		{
			m_pGaugeM->Set((float)m_nLife, D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));
		}

		//モーション更新
		if (m_pMotion != nullptr)
		{
			m_pMotion->Update(m_nNumModel, m_apModel);
		}
	}
}
//===============================================
// 描画処理
//===============================================
void CBoss::Draw(void)
{
	// 使用しているなら
	if (m_bUse == true)
	{
		// デバッグフォント
		CDebugProc* pDegub = CManager::GetDebug();
		pDegub->Print("ボスの位置：%.1f,%.1f,%.1f", m_pos.x, m_pos.y, m_pos.z);
		pDegub->Print("ボスの角度：%.1f,%.1f,%.1f", m_rot.x, m_rot.y, m_rot.z);
		pDegub->Print("ボスの移動量：%.1f,%.1f,%.1f", m_move.x, m_move.y, m_move.z);
		pDegub->Print("ボスのサイズ：%.1f,%.1f,%.1f", m_size.x, m_size.y, m_size.z);
		pDegub->Print("ボスの攻撃回数：%d", m_nCntAttack);

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
				// 描画
				m_apModel[nCnt]->Draw();
			}
		}
	}
}
//===============================================
// 移動処理
//===============================================
void CBoss::Move(void)
{
	// フィールドの情報取得
	D3DXVECTOR3 FieldPos = CGame::GetField()->GetPos();				// 位置
	float fFieldWidth = CGame::GetField()->GetWidth() * 1.25f;		// 横幅（少し大きめに取る）
	float fFieldHeight = CGame::GetField()->GetHeight() * 1.25f;	// 奥行（少し大きめに取る）

	// ステージ四隅の座標
	static D3DXVECTOR3 CornerPos[4] = 
	{
		{ FieldPos.x - fFieldWidth, FieldPos.y, FieldPos.z + fFieldHeight },	// 左奥
		{ FieldPos.x - fFieldWidth, FieldPos.y, FieldPos.z - fFieldHeight },	// 左前
		{ FieldPos.x + fFieldWidth, FieldPos.y, FieldPos.z - fFieldHeight },	// 右前
		{ FieldPos.x + fFieldWidth, FieldPos.y, FieldPos.z + fFieldHeight },	// 右奥
	};

	// 現在の目的位置
	D3DXVECTOR3 Target = CornerPos[m_nMoveTarget];

	// 目的位置までの方向ベクトル
	D3DXVECTOR3 Dir = Target - m_pos;
	D3DXVec3Normalize(&Dir, &Dir);

	// 移動
	m_move = Dir * BOSS_SPEED;

	// 目標の向きを計算
	float fDestRotY = atan2f(Dir.x, Dir.z);
	float fDiff = fDestRotY - m_rot.y + D3DX_PI;

	// 角度を正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	if (fDiff > D3DX_PI)
	{
		fDiff -= D3DX_PI * 2.0f;
	}
	if (fDiff < -D3DX_PI)
	{
		fDiff += D3DX_PI * 2.0f;
	}

	// 目標の向きを設定
	m_rotDest.y = m_rot.y + fDiff * 0.1f;

	// 向きを変える
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.5f;

	//重力加算
	m_move.y -= GRAVITY;

	// 前回の位置保存
	m_posOld = m_pos;
	// 移動
	m_pos += m_move;

	// 地面判定
	if (m_pos.y <= 0.0f)
	{
		// 位置と移動量を０に
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	// 目的地に近づいたら次へ
	D3DXVECTOR3 DiffPos = Target - m_pos;
	if (D3DXVec3Length(&DiffPos) < 5.0f)
	{
		m_nMoveTarget = (m_nMoveTarget + 1) % 4; // 次の角へ
	}

	// モーション切り替え
	if (m_pMotion->GetType() != MOTION_MOVE && m_pMotion->GetBlend() == false)
	{
		m_pMotion->StartBlend(MOTION_MOVE, 10);	// 移動
	}

	// 手前に来た時
	if (m_nMoveTarget == 2)
	{
		// 全モデル
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			// 少し透過させる
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetAlphaColor(0.75f);
				m_apModel[nCnt]->SetColorAlpha(true);
			}
		}
	}
	else
	{
		//モデル全描画
		for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
		{
			// 透過させない
			if (m_apModel[nCnt] != nullptr)
			{
				m_apModel[nCnt]->SetColorAlpha(false);
			}
		}
	}
}
//===============================================
// 攻撃処理
//===============================================
void CBoss::Action(void)
{
	//----------------------------------------------
	// 体をプレイヤーの方に向ける
	//----------------------------------------------
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos =  pPlayer->GetPos();

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toPlayer = PlayerPos - m_pos;
	
	// 正規化
	D3DXVec3Normalize(&toPlayer, &toPlayer);
	
	//角度
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotDest.y = angle + D3DX_PI;

	// 角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// 向きを変える
	m_rot += (m_rotDest - m_rot) * 0.5f;
	
	//-------------------------------------------
	// 攻撃パターンからどの攻撃をするか決定
	//-------------------------------------------
	switch (m_pAttack->GetType())
	{
	//---------------------------------------------
	// 投げつけ攻撃
	//---------------------------------------------
	case CBossAttack::TYPE_THROW:

		// モーション切り替え
		if (m_pMotion->GetType() != MOTION_THROW && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_THROW);	// 投げ
		}

		// 投げ攻撃設定
		m_pAttack->SetAttack(pPlayer->GetPos());
		m_pAttack->Throw();
		break;

	//---------------------------------------------
	// 薙ぎ払い攻撃
	//---------------------------------------------
	case CBossAttack::TYPE_SWEEP:

		// モーション切り替え
		if (m_pMotion->GetType() != MOTION_SWEEP && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_SWEEP);	// 薙ぎ払い
		}

		// 薙ぎ払い攻撃設定
		m_pAttack->SetAttack(pPlayer->GetPos());
		m_pAttack->Sweep();
		break;
		
	//---------------------------------------------
	// 火を吐く攻撃
	//---------------------------------------------
	case CBossAttack::TYPE_FIRE:
	{
		// モーション切り替え
		if (m_pMotion->GetType() != MOTION_MOVE && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_MOVE);	// 移動
		}

		// 顔の位置を算出
		D3DXVECTOR3 facepos = m_pos + (m_apModel[0]->GetOffPos() + m_apModel[1]->GetOffPos());
		facepos.y += 50.0f;

		// 火炎放射攻撃設定
		m_pAttack->SetAttack(pPlayer->GetPos());
		m_pAttack->Fire();

		// 攻撃状態なら
		if (m_bAction == true && m_bAttackCancel == false)
		{
			// 火炎放射パーティクル生成＆目標位置設定
			CParticle::Create(facepos, VECTOR3_NULL, D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f), 10, 40, CParticle::TYPE_FLAME);
			CParticle::SetTargetPos(m_pAttack->GetArea()->GetPos());
		}

		break;
	}
	//---------------------------------------------
	// 雷攻撃
	//---------------------------------------------
	case CBossAttack::TYPE_THUNDER:

		// モーション切り替え
		if (m_pMotion->GetType() != MOTION_THROW && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_THROW);	// 投げ
		}

		// 雷攻撃設定
		m_pAttack->SetAttack(pPlayer->GetPos());
		m_pAttack->Thunder();
		break;

	//---------------------------------------------
	// 振り下ろし攻撃
	//---------------------------------------------
	case CBossAttack::TYPE_SLAM:

		// モーション切り替え
		if (m_pMotion->GetType() != MOTION_THROW && m_pMotion->GetBlend() == false)
		{
			m_pMotion->StartBlend(MOTION_THROW);	// 投げ
		}

		m_pAttack->SetAttack(pPlayer->GetPos());
		m_pAttack->Throw();
		break;

	default:
		break;
	};
}
//===============================================
// 花瓶を調べる
//===============================================
void CBoss::ChekFlowerVase(void)
{
	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();		// 次のオブジェクトを取得
		CObject::TYPE type = pObj->GetObjType();	// 種類を取得

		// 種類が一致していれば
		if (type == CObject::TYPE_FLOWERVASE)
		{
			// 花瓶へのポインタ
			CFlowerVase* pFlowerVase = (CFlowerVase*)pObj;

			// 花束があれば
			if (pFlowerVase->GetHaveBouquet() == true)
			{
				// 攻撃キャンセル
				m_bAttackCancel = true;
				return;	// 終わる
			}
		}
		// 次のオブジェクトへ
		pObj = pObjNext;
	}
}
//===============================================
// ダメージ処理
//===============================================
void CBoss::Hit(const int nDamage)
{
	// NULLチェック
	if (this != nullptr && CGame::GetType() == CGame::TYPE_GAME)
	{
		// ダメージ分HPを減らす
		m_nLife -= nDamage;

		// HPが０なら
		if (m_nLife <= 0)
		{
			m_nLife = 0;	// ０にする
		}

		m_pGaugeM->SetShakeInfo(true, 30, D3DXVECTOR3(5.0f, 5.0f, 5.0f));
	}
}
//===============================================
// 状態遷移処理
//===============================================
void CBoss::State(STATE state)
{
	// 攻撃しないなら
	if (m_bAction == false)
	{
		// 状態を代入
		m_State = state;

		switch (m_State)
		{
			// 通常
		case STATE_NONE:
			m_nCntState = 30;	// カウンターを加算
			break;
			// 死亡
		case STATE_DEAD:
			m_bUse = false;		// 使用していない
			break;

		default:
			break;
		}
	}
}
//===============================================
// 攻撃設定
//===============================================
void CBoss::SetAction(bool bAction)
{
	// 攻撃設定
	m_bAction = bAction; 

	// 攻撃するなら
	if (bAction == true)
	{
		// カウンターを加算
		m_nCntAttack++;
	}
	// 攻撃しないなら
	else if (bAction == false)
	{
		// AIをクールダウン
		m_pAI->SetCoolDown();
	}
}
//===============================================
// ゲーム終了時にデータを保存する処理
//===============================================
void CBoss::EndSave(void)
{
	std::vector<int> saveData;

	saveData.push_back(m_nLife);					// HP
	saveData.push_back(m_pAI->GetNumMiss());		// ミス数
	saveData.push_back(m_pAI->GetNumAttack());		// 攻撃回数

	CFile::IntSave("data\\TEXT\\ResultSave.txt", saveData);
}