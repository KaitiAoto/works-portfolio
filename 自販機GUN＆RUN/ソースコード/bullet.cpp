//==============================
//
// 弾[bullet.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "enemy.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "collision.h"
#include "easing.h"

//==================
// コンストラクタ
//==================
CBullet::CBullet(int nPriority):CObject(nPriority)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_pModel = NULL;
	m_type = TYPE_CAN;
	m_user = USER_PLAYER;
	m_nLife = 0;
	m_bUse = true;
	m_bSkill = false;
}
//================
// デストラクタ
//================
CBullet::~CBullet()
{

}
//===========
// 生成処理
//===========
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user)
{
	// 生成
	CBullet* pBullet = new CBullet;
	// 初期化
	if (FAILED(pBullet->Init(pos, rot, user)))
	{// NULLチェック
		delete pBullet;
		return nullptr;
	}
	return pBullet;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user)
{
	// 値を代入
	m_pos = pos;
	m_rot = rot;
	m_user = user;

	// メンバ変数を初期化
	m_bUse = true;		// 使用している状態にする
	m_bSkill = false;	// スキルではない状態にする

	// モデル名格納用変数＆その初期化
	const char* pFilename = "data\\MODEL\\can00.x";
	
	// 移動量を設定
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * BULLET_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * BULLET_SPEED);

	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{
		pFilename = SetPlayerBullet();	// モデル名設定
		m_move.y = 5.0f;				// Ｙ軸の移動量を変更
		m_nLife = BULLET_LIFE;			// 寿命を設定
	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{
		m_nLife = BULLET_LIFE / 7;	// 寿命を設定
	}

	// モデル生成
	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	// サイズを設定
	m_size = m_pModel->SetSize() * 2;

	// オブジェクトの種類設定
	SetObjType(TYPE_BULLET);
	
	return S_OK;
}
//============
// 終了処理
//============
void CBullet::Uninit(void)
{	
	if (m_pModel != NULL)
	{// NULLチェック
		// モデルの終了処理＆破棄
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============
// 更新処理
//============
void CBullet::Update(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			if (CGame::GetMode() == CGame::MODE_TUTORIAL)
			{
				if (m_pos.x > CGame::GetStart()->GetPos().x)
				{
					m_bUse = false;
				}
			}

			// バウンド移動
			MoveBounce();

			// エフェクト生成
			CEffect::Create(
				D3DXVECTOR3(m_pos.x, m_pos.y + 1.0f, m_pos.z), 
				m_rot, 
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DCOLOR_RGBA(1, 127, 255, 255), 
				60, 
				EFFECT_SIZE);
		}
		// ユーザーが敵なら
		else if(m_user == USER_ENEMY)
		{
			// 移動
			Move();
		}
		// 当たり判定
		Collision();

		// 寿命を減らす
		m_nLife--;
		// 寿命が０以下なら
		if (m_nLife <= 0)
		{
			m_bUse = false;	// 使用していない状態に
		}
	}
	// 使っていないなら
	else if (m_bUse == false)
	{
		// 終了処理
		Uninit();
		return;
	}
}
//============
// 描画処理
//============
void CBullet::Draw(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxTrans;
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			// モデル描画
			m_pModel->Draw();
		}
	}
}
//===========
// 移動処理
//===========
void CBullet::Move(void)
{
	// 位置更新
	m_pos += m_move;

	// モデルの位置更新
	m_pModel->Set(m_pos, m_rot);
}
//==================
// バウンド移動
//==================
void CBullet::MoveBounce(void)
{
	const float fRestitution = 0.8f;	// 反発係数
	const float fMinY = 0.2f;			// Ｙ軸の最低移動量

	// 重力加算
	m_move.y -= GRAVITY;

	// 位置更新
	m_pos += m_move;

	// 地面との衝突
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;	// 位置を地面と同じにする

		// Y速度の反転
		m_move.y *= -fRestitution;

		// 移動量が一定量小さくなったら
		if (fabs(m_move.y) < fMinY)
		{
			m_move.y = 0.0f;	// Y軸の移動量を０に
		}
	}

	// モデルの位置更新
	m_pModel->Set(m_pos, m_rot);
}
//====================
// 敵との当たり判定
//====================
bool CBullet::Collision(void)
{
	// 当たり判定のポインタ取得
	CCollision* pColl = CGame::GetColl();

	D3DXVECTOR3 contactNormal;	// 接触法線

	// 当たり判定検知用
	bool bCollEnemy = false;	// 敵サイド
	bool bCollPlayer = false;	// プレイヤー
	bool bCollStage = false;	// ステージ
	bool bCollVender = false;	// 自販機
	bool bAnyColl = false;		// 上記の判定があるかを調べる用

	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{
		// 敵サイドとの判定
		//if (m_bSkill != true)
		{
			bCollEnemy = pColl->ToEnemy(m_pos, m_rot, m_size, TYPE_BULLET, contactNormal);
		}
		// 自販機との判定
		bCollVender = pColl->ToVender(m_pos, m_rot, m_size, TYPE_BULLET, contactNormal);
		// ステージとの判定
		bCollStage = pColl->ToStage(m_pos, m_rot, m_size, contactNormal,TYPE_BULLET);
	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{
		// プレイヤーとの判定
		bCollPlayer = pColl->EnemyBullet(m_pos, m_size);
	}

	// 何かに当たったら
	if (bCollEnemy == true || bCollVender == true || bCollStage == true || bCollPlayer == true)
	{
		// 終了処理
		Uninit();

		bAnyColl = true;	// 判定有り状態に
	}

	return bAnyColl;	 // 判定を返す
}
//======================================
// プレイヤーの弾の種類ごとのモデル名
//======================================
const char* CBullet::SetPlayerBullet(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// モデル名格納用変数＆その初期化
	const char* pFilename = "data\\MODEL\\can00.x";

	// 今の弾の種類を取得
	m_type = pPlayer->GetMyBullet();

	// 種類ごとにモデル名を取得
	switch (m_type)
	{
	case TYPE_CAN:
		pFilename = "data\\MODEL\\can00.x";
		break;
	case TYPE_ICE:
		pFilename = "data\\MODEL\\ice00.x";
		break;
	case TYPE_CAPSULE:
		pFilename = "data\\MODEL\\capsule00.x";
		break;
	case TYPE_PETBOTTLE:
		pFilename = "data\\MODEL\\pet_bottle00.x";
		break;
	case TYPE_DUST:
		pFilename = "data\\MODEL\\can01.x";
		break;
	case TYPE_SNACK:
		pFilename = "data\\MODEL\\snack00.x";
		break;
	case TYPE_CIGARET:
		pFilename = "data\\MODEL\\cigarette00.x";
		break;
	case TYPE_CARD:
		pFilename = "data\\MODEL\\card00.x";
		break;
	case TYPE_BOTTLE:
		pFilename = "data\\MODEL\\bottle00.x";
		break;

	default:
		break;
	}
	
	return pFilename;	// モデル名を返す
}