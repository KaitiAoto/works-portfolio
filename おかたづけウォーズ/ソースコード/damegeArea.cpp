//===============================================
//
// ダメージエリア処理[damegeArea.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "damegeArea.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "particle.h"

//===============================================
// コンストラクタ
//===============================================
CDamegeArea::CDamegeArea()
{
}
//===============================================
// デストラクタ
//===============================================
CDamegeArea::~CDamegeArea()
{
}
//===============================================
// 生成処理
//===============================================
CDamegeArea* CDamegeArea::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int nDamege, int nDamegeTime)
{
	// インスタンス生成
	CDamegeArea* pMesh = new CDamegeArea;

	// 値を代入
	pMesh->m_nDamege = nDamege;
	pMesh->m_nDamegeTime = nDamegeTime;
	
	pos.y = 0.2f;	// 位置微調整

	//初期化
	if (FAILED(pMesh->Init(pos, rot, fRadius, col, type)))
	{
		delete pMesh;
		return nullptr;
	}

	return pMesh;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CDamegeArea::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type)
{
	// 最大サイズ用の円を生成
	m_pMaxSize = CMeshCircle::Create(pos, VECTOR3_NULL, fRadius, D3DXCOLOR(col.r, col.g, col.b, 0.5f), CMeshCircle::TYPE_OUT);

	// 値を代入＆初期化
	m_fMaxRadius = fRadius;
	m_type = type;
	m_fNowRadius = 1.0f;
	m_bCntTime = false;

	pos.y += 0.1f;	// 位置微調整

	// 親クラスの初期化
	CMeshCircle::Init(pos, rot, m_fNowRadius, col, CMeshCircle::TYPE_OUT);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CDamegeArea::Uninit(void)
{
	// 親クラスの終了処理
	CMeshCircle::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CDamegeArea::Update(void)
{
	// 攻撃キャンセル状態なら
	if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		// 円を消して終了
		m_pMaxSize->Uninit();
		// 終了
		Uninit();
		
		return;
	}

	switch (m_type)
	{
	// 通常
	case TYPE_NONE:
		UpdateNone();
		break;
	// 火炎放射
	case TYPE_FIRE:
		UpdateFire();
		break;
	// 雷
	case TYPE_THUNDER:
		UpdateThunder();
		break;
	default:
		break;
	}
}
//===============================================
// 通常の更新
//===============================================
void CDamegeArea::UpdateNone(void)
{
	// ダメージを受けるまでの時間カウントしないなら
	if (m_bCntTime == false)
	{
		// 半径拡大
		m_fNowRadius += m_fMaxRadius / (float)m_nDamegeTime;
		// 半径が最大以上になったら
		if (m_fNowRadius >= m_fMaxRadius)
		{
			m_fNowRadius = m_fMaxRadius;
			// カウントするように
			m_bCntTime = true;
		}
		// サイズを設定
		SetSize(m_fNowRadius);
	}
	// ダメージを受けるまでの時間カウントするなら
	else
	{
		// ダメージ受付
		// エリア内にいたら
		if (CollPlayer() == true)
		{
			// プレイヤー情報取得
			CPlayer* pPlayer = CGame::GetPlayer();
			pPlayer->Hit(m_nDamege); // ダメージ
		}

		// インタラクトとの判定
		CollInteract();

		// 円を消して終了
		m_pMaxSize->Uninit();
		m_pMaxSize = nullptr;
		// 終了
		Uninit();
	}
}
//===============================================
// 火炎放射の更新
//===============================================
void CDamegeArea::UpdateFire(void)
{
	// 移動量
	const static float MOVESPEED = 1.5f;
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// 最大サイズ用の円は見えなくする
	m_pMaxSize->SetUse(false);	

	// 半径拡大
	m_fNowRadius = m_fMaxRadius;
	// サイズを設定
	SetSize(m_fNowRadius);

	//------------------------------------
	// 当たり判定
	//------------------------------------
	// エリア内にいたら
	if (CollPlayer() == true)
	{
		pPlayer->Hit(m_nDamege); // ダメージ
	}
	// インタラクトオブジェクトとの判定
	CollInteractFire();

	//------------------------------------
	// 移動
	//------------------------------------
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();	// プレイヤーの位置取得

	// プレイヤーへの方向ベクトル
	D3DXVECTOR3 toPlayer = PlayerPos - GetPos();
	// 正規化
	D3DXVec3Normalize(&toPlayer, &toPlayer);

	//移動量
	D3DXVECTOR3 move = VECTOR3_NULL;
	move.x = toPlayer.x * MOVESPEED;
	move.z = toPlayer.z * MOVESPEED;

	// 位置更新
	SetPos(GetPos() + move);

	// カウンターを加算
	m_nCntFireTime++;

	// 一定時間経過したら
	if (m_nCntFireTime >= m_nDamegeTime)
	{
		m_nCntFireTime = 0;	// カウンターをクリア

		// 円を消して終了
		m_pMaxSize->Uninit();
		m_pMaxSize = nullptr;
		// 終了
		Uninit();

		return;
	}
}
//===============================================
// 雷の更新
//===============================================
void CDamegeArea::UpdateThunder(void)
{
	// 移動量
	const static float MOVESPEED = 1.5f;
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// ダメージを受けるまでの時間カウントしないなら
	if (m_bCntTime == false)
	{
		//------------------------------------
		// プレイヤーに追従
		//------------------------------------
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

		// プレイヤーへの方向ベクトル
		D3DXVECTOR3 toPlayer = PlayerPos - GetPos();
		// 正規化
		D3DXVec3Normalize(&toPlayer, &toPlayer);

		//移動量
		D3DXVECTOR3 move = VECTOR3_NULL;
		move.x = toPlayer.x * MOVESPEED;
		move.z = toPlayer.z * MOVESPEED;

		// 位置更新
		m_pMaxSize->SetPos(m_pMaxSize->GetPos() + move);
		SetPos(GetPos() + move);

		//------------------------------------
		// 半径拡大
		//------------------------------------
		m_fNowRadius += m_fMaxRadius / (float)m_nDamegeTime;
		// 半径が最大以上になったら
		if (m_fNowRadius >= m_fMaxRadius)
		{
			m_fNowRadius = m_fMaxRadius;
			// カウントするように
			m_bCntTime = true;
		}
		// サイズを設定
		SetSize(m_fNowRadius);
	}
	// ダメージを受けるまでの時間カウントするなら
	else
	{
		// ダメージ受付
		// エリア内にいたら
		if (CollPlayer() == true)
		{
			pPlayer->Hit(m_nDamege); // ダメージ
			pPlayer->SetStun(60);
		}

		// パーティクル生成
		CParticle::Create(D3DXVECTOR3(GetPos().x, 500.0f, GetPos().z), VECTOR3_NULL, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 1, 200, CParticle::TYPE_THUNDER);
		CParticle::SetTargetPos(GetPos());

		// インタラクトとの判定
		CollInteract();

		// 円を消して終了
		m_pMaxSize->Uninit();
		m_pMaxSize = nullptr;
		// 終了
		Uninit();
	}
}
//===============================================
// 描画処理
//===============================================
void CDamegeArea::Draw(void)
{
	// 親クラスの終了処理
	CMeshCircle::Draw();
}
//===============================================
// プレイヤーとの判定
//===============================================
bool CDamegeArea::CollPlayer()
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerSize = pPlayer->GetSize();

	// 半径をサイズから算出
	float fPlayerRadius = max(PlayerSize.x, max(PlayerSize.y, PlayerSize.z)) * 0.5f;

	// 半径での判定
	if (CManager::GetColl()->Radius(PlayerPos, fPlayerRadius, CMeshCircle::GetPos(), CMeshCircle::GetRadius()) == true)
	{// 範囲内なら
		return true;
	}
	return false;
}
//===============================================
// インタラクトオブジェクトとの判定
//===============================================
void CDamegeArea::CollInteract(void)
{
	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();		// 次のオブジェクト取得
		CObject::TYPE type = pObj->GetObjType();	// 種類を取得

		// 種類が一致していれば
		if (type == CObject::TYPE_PHYSICS || type == CObject::TYPE_INTERACT)
		{
			// 物理オブジェクト情報取得
			CObjectPhysics* pObjPhysics = (CObjectPhysics*)pObj;
			D3DXVECTOR3 PhysicsPos = pObjPhysics->GetPos();		// 位置
			D3DXVECTOR3 PhysicsSize = pObjPhysics->GetSize();	// サイズ

			// 半径をサイズから算出
			float fRadius = max(PhysicsSize.x, max(PhysicsSize.y, PhysicsSize.z)) * 0.5f;

			// 半径での判定
			if (CManager::GetColl()->Radius(PhysicsPos, fRadius, CMeshCircle::GetPos(), CMeshCircle::GetRadius()) == true)
			{// 範囲内なら
				// ベクトル算出
				D3DXVECTOR3 vec = PhysicsPos - CMeshCircle::GetPos();
				D3DXVec3Normalize(&vec, &vec);

				// 距離を取得
				D3DXVECTOR3 diff = PhysicsPos - CMeshCircle::GetPos();
				float fDist = D3DXVec3Length(&diff);
				float fMaxRadius = CMeshCircle::GetRadius();

				// 範囲に応じて威力減少
				float fPower = 10.0f;	// 最大吹き飛ばし力
				float fRate = 1.0f - (fDist / fMaxRadius);
				if (fRate < 0.0f)
				{
					fRate = 0.0f;
				}
				// 吹き飛ばしベクトルを算出
				D3DXVECTOR3 move = vec * (fPower * fRate);

				// 上方向
				move.y += fPower * 0.3f;

				// 吹き飛ばし
				pObjPhysics->SetBlow(false, move);
			}
		}
		// 次のオブジェクトへ
		pObj = pObjNext;
	}
}
//===============================================
// インタラクトオブジェクトと火炎放射との判定
//===============================================
void CDamegeArea::CollInteractFire(void)
{
	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();		// 次のオブジェクト取得
		CObject::TYPE type = pObj->GetObjType();	// 種類を取得

		// 種類が一致していれば
		if (type == CObject::TYPE_INTERACT)
		{
			// 物理オブジェクト情報取得
			CInteract* pObjInteract = (CInteract*)pObj;
			D3DXVECTOR3 InteractPos = pObjInteract->GetPos();
			D3DXVECTOR3 InteractSize = pObjInteract->GetSize();
			// 半径をサイズから算出
			float fRadius = max(InteractSize.x, max(InteractSize.y, InteractSize.z)) * 0.5f;

			// 半径での判定
			if (CManager::GetColl()->Radius(InteractPos, fRadius, CMeshCircle::GetPos(), CMeshCircle::GetRadius()) == true)
			{// 範囲内なら

				// 灰じゃないなら
				if (pObjInteract->GetbAsh() == false)
				{
					pObjInteract->ChangeAsh();	// 灰にする
				}
			}
		}
		// 次のオブジェクトへ
		pObj = pObjNext;
	}
}