//==============================
//
// 敵[enemy.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "collision.h"
#include "matchup.h"
#include "score_manager.h"
//
int CEnemy::m_nCntKill = 0;
//==================
// コンストラクタ
//==================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_type = TYPE_LIGHTER;

	m_bUse = true;
	
	
	m_State = STATE_NONE;
	m_nCntState = 0;

	m_pMyGroup = nullptr;
}
//================
// デストラクタ
//================
CEnemy::~CEnemy()
{

}
//===========
// 生成処理
//===========
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CEnemy* pBullet = new CEnemy;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pos, rot, type)))
	{
		delete pBullet;
		return nullptr;
	}
	else
	{
		pBullet->m_pMyGroup = nullptr;
	}

	return pBullet;
}
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, CEnemyGroup* Group)
{
	CEnemy* pBullet = new CEnemy;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pos, rot, type)))
	{
		delete pBullet;
		return nullptr;
	}
	else
	{
		pBullet->m_pMyGroup = Group;
	}

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CEnemy::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * 1.5f, 3.0f, cosf(m_rot.y - D3DX_PI) * 1.5f);
	m_nLife = ENEMY_LIFE;
	m_bUse = true;
	m_type = type;
	m_State = STATE_STAY;
	m_nStayCounter = rand() % 120 + 60;
	m_fRandomAngle = D3DXToRadian(rand() % 360);

	//モデル生成
	const char* pFilename = "data\\MODEL\\lighter00.x";
	pFilename = SetModelName();//モデル名

	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	m_size = m_pModel->SetSize();
	
	m_posHalf = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z);

	float GeuseBase = m_nLife / 10.0f;
	m_pGauge = CEnemyGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 1.5f), m_pos.z), GeuseBase, 5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	m_pShadow = CShadow::Create(m_pos, m_rot, m_size.x, m_size.z, CShadow::TYPE_CIRCLE);

	//オブジェクトの種類設定
	SetObjType(TYPE_ENEMY);

	return S_OK;
}
//============
// 終了処理
//============
void CEnemy::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	if (m_pGauge != NULL)
	{
		m_pGauge->Uninit();
	}

	CObject::Release();
}
//============
// 更新処理
//============
void CEnemy::Update(void)
{
	if (CGame::GetMode() != CGame::MODE_TUTORIAL)
	{
		if (m_bUse == true)
		{
			float rate = (float)m_nLife / (float)ENEMY_LIFE;
			rate = max(0.0f, min(rate, 1.0f));

			m_pGauge->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y * 1.5f), m_pos.z));
			m_pGauge->SetRate(rate);

			switch (m_State)
			{
			case STATE_STAY:
				//移動
				Move();
				break;
			case STATE_MOVE:
				//移動
				Move();
				break;
			case STATE_HIT:

				m_move.y -= GRAVITY; //重力加算

				// 前回の位置保存
				m_posOld = m_pos;
				// 移動
				m_pos += m_move;
				// 地面判定
				if (m_pos.y <= 0.0f)
				{
					m_pos.y = 0.0f;
					m_move.y = 0.0f;
				}
				m_posHalf = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z);
				m_pModel->Set(m_pos, m_rot);


				m_nCntState--;
				if (m_nCntState <= 0)
				{
					m_nCntState = 0;
					State(STATE_STAY);
				}
				break;

			default:
				break;
			}

			if (m_State != STATE_HIT)
			{
				m_pModel->SetColorChange(false);
			}
			if (m_State == STATE_STAY)
			{
				m_pGauge->SetDraw(false);
			}
			else
			{
				m_pGauge->SetDraw(true);
			}

			//当たり判定
			Collision();


			if (m_pShadow != nullptr)
			{
				m_pShadow->SetPos(D3DXVECTOR3(m_pos.x, 0.3f, m_pos.z));
				m_pShadow->SetRot(m_rot);
			}

			//寿命
			if (m_nLife <= 0)
			{
				CParticle::Create(m_posHalf, m_rot, D3DCOLOR_RGBA(255, 1, 1, 255), 15, 5, CParticle::TYPE_NONE);

				m_bUse = false;
				m_pGauge->SetDraw(false);

				if (m_pMyGroup != nullptr)
				{
					m_pMyGroup->SubMyEnemy();
				}
				m_pShadow->SetUse(false);
			}
		}
		else if (m_bUse == false)
		{//使っていないなら
			m_nCntKill++;
			if (m_nCntKill >= 5)
			{
				CGame::GetBuff()->AddSpeed(PLAYER_SPEED * 1.5f, 5);
				m_nCntKill = 0;
			}

			Uninit();
		}
	}
}
//============
// 描画処理
//============
void CEnemy::Draw(void)
{
	if (CGame::GetMode() != CGame::MODE_TUTORIAL)
	{
		if (m_bUse == true)
		{
			//デバイスの取得
			CRenderer* pRenderer = CManager::GetRenderer();
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

			//計算用マトリックス
			D3DXMATRIX mtxRot, mtxTrans;
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxWorld);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

			//モデル描画
			m_pModel->Draw();
		}
	}
}
//===========
// 移動処理
//===========
void CEnemy::Move(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

	// プレイヤーに向かって動く
	if (m_State == STATE_MOVE)
	{
		if(m_type == TYPE_TORNADO || m_type == TYPE_MAGNET || m_type == TYPE_MONEY1)
		{
			Move_Charge();	// 突進
		}
		else if (m_type == TYPE_RANGE || m_type == TYPE_KETTLE || m_type == TYPE_MONEY0)
		{
			Move_Jump();	// ジャンプ
		}
		else if (m_type == TYPE_LIGHTER || m_type == TYPE_SPRAY || m_type == TYPE_EXT)
		{
			Move_Shot();	// ショット
		}
		else
		{
			Move_Normal();	// 通常
		}
	}
	// ランダムに動く
	else if (m_State == STATE_STAY)
	{	
		m_nStayCounter--;
		if (m_nStayCounter <= 0)
		{
			// 一定時間ごとに方向を更新
			m_fRandomAngle = D3DXToRadian(rand() % 360);
			m_nStayCounter = rand() % 120 + 60;
		}

		float moveSpeed = 0.05f;
		m_move.x = cosf(m_fRandomAngle) * moveSpeed;
		m_move.z = sinf(m_fRandomAngle) * moveSpeed;

		m_rotDest.y = atan2f(m_move.x, m_move.z) + D3DX_PI;
	}

	// プレイヤーとの距離と自身の検知する半径を求める
	D3DXVECTOR3 toPlayer = PlayerPos - m_pos;
	float distance = D3DXVec3Length(&toPlayer);
	float fRadius = max(m_size.x, max(m_size.y, m_size.z)) * 10.0f;

	// 範囲内なら
	if (distance <= fRadius)
	{
		State(STATE_MOVE);
	}
	// 範囲外なら
	else if (distance > fRadius)
	{
		State(STATE_STAY);
	}

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

	m_move.y -= GRAVITY; //重力加算

	// 前回の位置保存
	m_posOld = m_pos;
	// 移動
	m_pos += m_move;

	// 地面判定
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	m_posHalf = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z);
	m_pModel->Set(m_pos, m_rot);
}
//================
// 通常移動処理
//================
void CEnemy::Move_Normal(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toPlayer = PlayerPos - m_pos;
	// 正規化
	D3DXVec3Normalize(&toPlayer, &toPlayer);

	//移動量
	m_move.x = toPlayer.x * ENEMY_SPEED;
	m_move.z = toPlayer.z * ENEMY_SPEED;
	////角度
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotDest.y = angle + D3DX_PI;
}
//===============
// 突進移動処理
//===============
void CEnemy::Move_Charge(void)
{
	static bool bCharging = false;
	static int chargeTimer = 0;
	
	const float chargeframe = 120.0f;
	const int chargeTime = 90;
	const int waitTime = 120;

	// プレイヤーの情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPos();

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toPlayer = playerPos - m_pos;
	float distance = D3DXVec3Length(&toPlayer);
	D3DXVec3Normalize(&toPlayer, &toPlayer);

	if (!bCharging)
	{
		float chargeDistance = distance * 2.0f;
		float Speed = chargeDistance / chargeframe;

		m_move.x = toPlayer.x * Speed;
		m_move.z = toPlayer.z * Speed;
		m_rotDest.y = atan2f(toPlayer.x, toPlayer.z) + D3DX_PI;

		bCharging = true;
		chargeTimer = chargeTime; 
	}
	else
	{
		if (chargeTimer > 0)
		{
			chargeTimer--;

			CEffect::Create(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 0, 10);
		}
		else
		{
			m_move.x = 0.0f;
			m_move.z = 0.0f;

			static int waitTimer = 0;
			waitTimer++;

			if (waitTimer > waitTime)
			{
				waitTimer = 0;
				bCharging = false; 
			}
		}
	}
}
//=====================
// ジャンプ移動処理
//=====================
void CEnemy::Move_Jump(void)
{
	static int state = 0;       // 0: 接近, 1: ジャンプ, 2: 待機
	static int timer = 0;
	static bool canJump = true;

	const float JumpRange = 75.0f;
	const float JumpMove = 22.5f;
	const float JumpFrame = 30.0f;
	const int waitTime = 60;

	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPos();
	D3DXVECTOR3 toPlayer = playerPos - m_pos;
	float distance = D3DXVec3Length(&toPlayer);

	switch (state)
	{
	case 0: // 接近
	{
		D3DXVec3Normalize(&toPlayer, &toPlayer);
		m_move.x = toPlayer.x * ENEMY_SPEED;
		m_move.z = toPlayer.z * ENEMY_SPEED;
		m_rotDest.y = atan2f(toPlayer.x, toPlayer.z) + D3DX_PI;

		if (distance < JumpRange && canJump)
		{
			float jumpSpeedH = distance / JumpFrame;
			m_move.x = toPlayer.x * jumpSpeedH;
			m_move.z = toPlayer.z * jumpSpeedH;

			m_move.y = JumpMove;

			canJump = false;
			state = 1;
		}
	}
	break;

	case 1: // ジャンプ中
		m_move.y -= GRAVITY;  // 重力加算

		CEffect::Create(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0, 15);

		if (m_pos.y <= 0.0f)
		{
			m_pos.y = 0.0f;
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			timer = waitTime;
			state = 2;
		}
		break;

	case 2: // 待機中
		timer--;
		if (timer <= 0)
		{
			canJump = true;
			state = 0;
		}
		break;
	}
}
//====================
// ショット移動処理
//====================
void CEnemy::Move_Shot(void)
{
	const float AttackRange = 80.0f;  // 射程距離
	const float StopSpeed = 0.0f;      // 射撃時は止まる
	const float MoveSpeed = ENEMY_SPEED;
	const int ShotTime = 60;

	static int ShotTimer = 0;

	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerPos = pPlayer->GetPos();
	D3DXVECTOR3 toPlayer = playerPos - m_pos;
	float distance = D3DXVec3Length(&toPlayer);


	if (ShotTimer > 0)
	{
		ShotTimer--;
	}

	if (distance <= AttackRange)
	{
		m_move = D3DXVECTOR3(0, 0, 0);

		// 敵の向きをプレイヤー方向に
		D3DXVECTOR3 dir = toPlayer;
		D3DXVec3Normalize(&dir, &dir);
		m_rotDest.y = atan2f(dir.x, dir.z) + D3DX_PI;

		if (ShotTimer <= 0)
		{
			CBullet::Create({ m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z }, m_rot, CBullet::USER_ENEMY);
			
			D3DXCOLOR col;
			if (m_type == TYPE_LIGHTER)
			{
				col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if(m_type == TYPE_SPRAY)
			{
				col = D3DXCOLOR(1.0f, 0.8f, 0.2f, 1.0f);
			}
			else if (m_type == TYPE_EXT)
			{
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// スプレーパーティクル生成
			CParticle::Create({ m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z }, m_rot, col, 1, 8.0f, CParticle::TYPE_SPRAY);

			ShotTimer = ShotTime;
		}
	}
	else
	{
		// 範囲外 → プレイヤーに向かって移動
		D3DXVECTOR3 dir = toPlayer;
		D3DXVec3Normalize(&dir, &dir);

		m_move.x = dir.x * MoveSpeed;
		m_move.z = dir.z * MoveSpeed;

		// 向きもプレイヤー方向に向ける
		m_rotDest.y = atan2f(dir.x, dir.z) + D3DX_PI;
	}
}
//====================
// 敵との当たり判定
//====================
bool CEnemy::Collision(void)
{
	//当たり判定のポインタ取得
	CCollision* pColl = CGame::GetColl();

	D3DXVECTOR3 contactNormal;

	bool bColl = pColl->ToPlayer(m_pos, m_size,this);
	//自販機との判定
	bool bCOLL = pColl->ToVender(m_pos, m_rot, m_size, TYPE_ENEMY, contactNormal);
	//
	bool bCollStage = pColl->ToStage(m_pos, m_rot, m_size, contactNormal,TYPE_ENEMY);
	//
	bool bCollGimmick = pColl->ToGimmick(m_pos, m_size, TYPE_ENEMY);

	if (bColl == true || bCOLL == true || bCollStage == true)
	{
		m_pos = m_posOld;
	}

	if (bCollGimmick == true)
	{
		Hit(1);
	}

	return bColl;
}
//================
// ダメージ処理
//================
void CEnemy::Hit(const int nDamage)
{
	CSound* pSound = CManager::GetSound();
	CScoreMana* pTotalScore = CGame::GetTotalScore();

	m_nLife -= nDamage;

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move.y += 7.0f;

	if (m_nLife > 0)
	{
		if (m_State != STATE_HIT)
		{
			m_pModel->SetColorChange(true);

			State(STATE_HIT);

			if (nDamage == BULLET_ATTACK)
			{//通常ダメ
				m_pModel->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
				
				CParticle::Create(m_posHalf, m_rot, D3DCOLOR_RGBA(255, 255, 1, 255), 5, 5, CParticle::TYPE_NONE);
				
				pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);

				pTotalScore->AddScore(200);
			}
			else if (nDamage == BULLET_ATTACK * ADVANTAGE)
			{//効果抜群
				m_pModel->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				
				CParticle::Create(m_posHalf, m_rot, D3DCOLOR_RGBA(255, 127, 1, 255), 5, 8, CParticle::TYPE_NONE);
				
				pSound->PlaySound(CSound::SOUND_LABEL_CRITICAL);

				pTotalScore->AddScore(400);
			}
			else if (nDamage == BULLET_ATTACK / DISADVANTAGE)
			{//効果いまひとつ
				m_pModel->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.9f, 1.0f));
				CParticle::Create(m_posHalf, m_rot, D3DCOLOR_RGBA(1, 127, 255, 255), 5, 3, CParticle::TYPE_NONE);
				pSound->PlaySound(CSound::SOUND_LABEL_RESIST);

				pTotalScore->AddScore(50);
			}
			else if (nDamage <= 0)
			{//ノーダメ
				//m_pModel->SetColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
				m_pModel->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.9f, 1.0f));

				pSound->PlaySound(CSound::SOUND_LABEL_NODAMAGE);
			}
			else
			{
				if (nDamage > 0)
				{
					m_pModel->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
					CParticle::Create(m_posHalf, m_rot, D3DCOLOR_RGBA(255, 255, 1, 255), 5, 5, CParticle::TYPE_NONE);
					pSound->PlaySound(CSound::SOUND_LABEL_DAMAGE);

					pTotalScore->AddScore(200);
				}
			}
		}
	}
	if (m_nLife <= 0)
	{
		State(STATE_DEAD);
		pSound->PlaySound(CSound::SOUND_LABEL_KILL);

		pTotalScore->AddScore(1000);
	}
}
//=================
// 状態遷移処理
//=================
void CEnemy::State(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NONE:
		m_pModel->SetColorChange(false);
		break;
	case STATE_HIT:
		m_nCntState = 45;
		break;
	case STATE_DEAD:
		ItemSet();
		m_bUse = false;
		break;

	default:
		break;
	}
}
//===================
// アイテムセット
//===================
void CEnemy::ItemSet()
{
	int nItem;
	srand((unsigned int)time(NULL));
	nItem = rand() % 3 + 1;

	if (nItem == 1)
	{// 1/3 でアイテム生成
		int itemType = rand() % 100 + 1;
		if (itemType <= 35)
		{
			CItem::Create({ m_pos.x,0.0f,m_pos.z }, m_rot, CItem::TYPE_LIFE);
		}
		else
		{
			CItem::Create({ m_pos.x,0.0f,m_pos.z }, m_rot, CItem::TYPE_BULLET);
		}
	}
}
//===========================
// タイプごとのモデル名設定
//===========================
const char* CEnemy::SetModelName(void)
{
	const char* pFilename = "data\\MODEL\\lighter00.x";

	switch (m_type)
	{
	case TYPE_LIGHTER:
		pFilename = "data\\MODEL\\lighter00.x";
		break;
	case TYPE_MONEY0:
		pFilename = "data\\MODEL\\money00.x";
		break;
	case TYPE_MONEY1:
		pFilename = "data\\MODEL\\money01.x";
		break;
	case TYPE_MAGNET:
		pFilename = "data\\MODEL\\magnet00.x";
		break;
	case TYPE_TORNADO:
		pFilename = "data\\MODEL\\tornado00.x";
		break;
	case TYPE_SPRAY:
		pFilename = "data\\MODEL\\spray00.x";
		break;
	case TYPE_EXT:
		pFilename = "data\\MODEL\\extinguisher00.x";
		break;
	case TYPE_KETTLE:
		pFilename = "data\\MODEL\\kettle00.x";
		break;
	case TYPE_RANGE:
		pFilename = "data\\MODEL\\microwave00.x";
		break;

	default:
		break;
	}

	return pFilename;

}