//===============================================
//
// 物理的なオブジェクト処理[objectPhysics.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "objectPhysics.h"
#include "manager.h"
#include "gatherArea.h"

// 静的メンバ変数
int CObjectPhysics::m_nNumAll = 0;

//===============================================
// コンストラクタ
//===============================================
CObjectPhysics::CObjectPhysics(int nPriority) : CObjectX(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR3_NULL;
	m_move = VECTOR3_NULL;
	m_blowMove = VECTOR3_NULL;
	m_posOld = VECTOR3_NULL;
	m_fWeight = 0.0f;

	m_bUseColl = true;
	m_bBlow = false;
	m_bThrow = false;
}
//===============================================
// デストラクタ
//===============================================
CObjectPhysics::~CObjectPhysics()
{
}
//===============================================
// 生成処理
//===============================================
CObjectPhysics* CObjectPhysics::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight)
{
	CObjectPhysics* pInteract;
	pInteract = new CObjectPhysics;
	
	//初期化
	pInteract->Init(pFilename, pos, rot, fWeight);

	m_nNumAll++;

	return pInteract;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CObjectPhysics::Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight)
{
	CObjectX::Init(pFilename, pos, rot);

	m_pos = pos;
	m_rot = rot;
	m_size = GetSize();
	m_fWeight = fWeight;
	m_bUseColl = true;

	m_move = VECTOR3_NULL;
	m_posOld = VECTOR3_NULL;

	SetObjType(TYPE_PHYSICS);//オブジェクトのタイプ

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CObjectPhysics::Uninit(void)
{
	// 全体の最大数を減らす
	m_nNumAll--;

	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CObjectPhysics::Update(void)
{
	CObjectX::Update();

	// 吹き飛ぶ
	if (m_bBlow == true)
	{
		// クォータニオン
		// 実際の移動量
		D3DXVECTOR3 deltaPos = m_pos - m_posOld;

		// 移動方向
		D3DXVECTOR3 moveDir = -deltaPos;
		D3DXVec3Normalize(&moveDir, &moveDir);

		// 回転軸（進行方向 × 上方向）
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXVECTOR3 vecAxis;
		D3DXVec3Cross(&vecAxis, &moveDir, &up);
		D3DXVec3Normalize(&vecAxis, &vecAxis);
		// 回転量（移動距離 / 半径）
		float radius = max(m_size.x, max(m_size.y, m_size.z)) * 0.75f;
		float moveLen = D3DXVec3Length(&deltaPos);
		float fValueRot = moveLen / radius;

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &vecAxis, fValueRot);

		SetQuat(true, quat);

		// 吹き飛ばし
		Blow();
	}
	m_move.y -= GRAVITY; //重力加算

	//前回の位置保存
	m_posOld = m_pos;

	//位置更新
	m_pos += m_move;

	// フィールドの情報取得
	D3DXVECTOR3 FieldPos = CGame::GetField()->GetPos();
	float fFieldHeight = CGame::GetField()->GetHeight();
	float fFieldWidth = CGame::GetField()->GetWidth();

	// 吹き飛ぶなら
	if(m_bBlow == true)
	{
		float fHeight = fFieldHeight;
		float fWidth = fFieldWidth;
		bool bBlow = true;

		if (m_bThrow == true)
		{
			fHeight *= 1.5f;
			fWidth *= 1.5f;
		}

		if (m_pos.x < FieldPos.x - fWidth || m_pos.x > FieldPos.x + fWidth)
		{
			bBlow = false;
		}
		if (m_pos.z < FieldPos.z - fHeight || m_pos.z > FieldPos.z + fHeight)
		{
			bBlow = false;
		}
		// Yが０以下
		if (m_pos.y < 0.0f)
		{
			bBlow = false;
		}

		if (bBlow == false)
		{
			m_pos = m_posOld;
			m_blowMove = VECTOR3_NULL;
			m_move = VECTOR3_NULL;

			m_bBlow = false;
			m_bThrow = false;
			bBlow = true;
		}
	}
	else if (m_bBlow == false)
	{
		// 当たり判定
		CollPlayer();
		CollSame();

		ReturnRotation();
		//// クォータニオン回転を無効化
		//SetQuat(false, D3DXQUATERNION(0, 0, 0, 1));

		// Xがフィールド範囲内
		if (m_pos.x > FieldPos.x - fFieldWidth && m_pos.x < FieldPos.x + fFieldWidth)
		{
			// Zがフィールド範囲内
			if (m_pos.z > FieldPos.z - fFieldHeight && m_pos.z < FieldPos.z + fFieldHeight)
			{
				// Yが０以下
				if (m_pos.y < 0.0f)
				{
					m_pos.y = 0.0f;
					m_move.y = 0.0f;
				}
			}
		}

		//移動量初期化
		m_move.x = 0.0f;
		m_move.z = 0.0f;

		// 下に落ちたら
		if (m_pos.y < -500.0f)
		{
			Uninit();
			return;
		}
	}

	CObjectX::SetPos(m_pos);
}
//===============================================
// 描画処理
//===============================================
void CObjectPhysics::Draw(void)
{
	CObjectX::Draw();
}
//===============================================
// 吹き飛ぶ処理
//===============================================
void CObjectPhysics::Blow()
{
	m_move += m_blowMove;

	static float DiffBlowXZ = 1.0f;
	static float DiffBlowY = 0.5f;

	//if (m_bThrow == false)
	//{
	//	DiffBlowXZ = 0.5f;
	//	DiffBlowY = 0.25f;
	//}
	
	// X吹き飛び量が正の数
	if (m_blowMove.x > 0.0f)
	{
		m_blowMove.x -= DiffBlowXZ;
		if (m_blowMove.x <= 0.0f) { m_blowMove.x = 0.0f; }
	}
	// X吹き飛び量が０以下
	else if (m_blowMove.x < 0.0f)
	{
		m_blowMove.x += DiffBlowXZ;
		if (m_blowMove.x >= 0.0f) { m_blowMove.x = 0.0f; }
	}
	// Y吹き飛び量が正の数
	if (m_blowMove.y > 0.0f)
	{
		m_blowMove.y -= DiffBlowY;
		if (m_blowMove.y <= 0.0f) { m_blowMove.y = 0.0f; }
	}
	// Y吹き飛び量が０以下
	else if (m_blowMove.y < 0.0f)
	{
		m_blowMove.y = 0.0f;
	}
	// Z吹き飛び量が正の数
	if (m_blowMove.z > 0.0f)
	{
		m_blowMove.z -= DiffBlowXZ;
		if (m_blowMove.z <= 0.0f) { m_blowMove.z = 0.0f; }
	}
	// Z吹き飛び量が０以下
	else if (m_blowMove.z < 0.0f)
	{
		m_blowMove.z += DiffBlowXZ;
		if (m_blowMove.z >= 0.0f) { m_blowMove.z = 0.0f; }
	}
}
//===============================================
// プレイヤーとの当たり判定
//===============================================
bool CObjectPhysics::CollPlayer(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	if (!pPlayer) return false;

	const Obb& playerObb = pPlayer->GetOBB();
	if (CManager::GetColl()->OBB(GetOBB(), playerObb))
	{
		// プレイヤーの移動量取得
		D3DXVECTOR3 PlayerMove = pPlayer->GetMove();

		// 重なり防止用ちょい押し出し
		D3DXVECTOR3 dir = m_pos - pPlayer->GetPos();
		if (D3DXVec3LengthSq(&dir) > 0.0001f)
		{
			D3DXVec3Normalize(&dir, &dir);
			float pushBack = 1.0f;
			m_pos += dir * pushBack;
			SetPos(m_pos - dir * pushBack);
		}

		m_move = PlayerMove;
		// プレイヤーと同じ移動をする
		m_pos += PlayerMove;

		// プレイヤーのスピードを重さによって変える
		pPlayer->SetSpeed(pPlayer->GetSpeed() / m_fWeight);

		// 自身のOBBを設定
		Obb Obb = GetOBB();
		Obb.centerPos = m_pos;
		SetOBB(Obb);

		return true;
	}

	return false;
}
//===============================================
// 同オブジェクトとの当たり判定
//===============================================
bool CObjectPhysics::CollSame(void)
{
	bool bColl = false;

	// プレイヤー取得
	CPlayer* pPlayer = CGame::GetPlayer();
	if (!pPlayer) return false;

	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();

		// 種類が一致していれば
		if (type == this->GetObjType() || type == CObject::TYPE_PHYSICS)
		{
			// プレイヤーの位置取得
			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

			// 距離がプレイヤーと近いもののみ調べる
			if (CManager::GetColl()->Radius(PlayerPos, CollRadius, GetPos(), 0.1f) == true || m_bUseColl == true)
			{
				CObjectPhysics* pObjPhy = (CObjectPhysics*)pObj;
				// 自分自身はスキップ
				if (pObjPhy == this || pObjPhy->GetBlow() == true)
				{
					pObj = pObjNext;
					continue;
				}
				// 自身より後に生成されたもののみ調べる
				if (this < pObjPhy)
				{
					const Obb& ObjObb = pObjPhy->GetOBB();
					if (CManager::GetColl()->OBB(GetOBB(), ObjObb))
					{
						// -----※要改善-----
						// お互いを半々ずつずらす！！
						// これできれいに行く!

						D3DXVECTOR3 push = VECTOR3_NULL;
						D3DXVECTOR3 dir = m_pos - pObjPhy->GetPos();
						if (D3DXVec3LengthSq(&dir) > 0.0001f)
						{
							D3DXVec3Normalize(&dir, &dir);
							float pushBack = 1.0f;
							m_pos += dir * pushBack;
							push = (dir * pushBack) / 2.0f;
							pObjPhy->SetPos(pObjPhy->GetPos() - push);
						}

						// 反発
						D3DXVECTOR3 tmp = m_move;
						m_move = pObjPhy->GetMove();
						pObjPhy->SetMove(tmp - push);

						bColl = true;
					}
				}
			}
		}

		pObj = pObjNext;
	}

	return bColl;
}