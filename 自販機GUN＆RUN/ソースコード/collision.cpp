//==============================
//
// 当たり判定[collision.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "collision.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"

//==================
// コンストラクタ
//==================
CCollision::CCollision()
{
}
//================
// デストラクタ
//================
CCollision::~CCollision()
{
}

//=============================================================================
// 
// 判定をとる相手の種類を調べる処理(どういうオブジェクトとの判定をとるか)
// 
//=============================================================================

//=========================
// 敵サイドとの当たり判定
//=========================
bool CCollision::ToEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal)
{
	bool bColl = false;

	CObject* pObj = CObject::GetTop(PRIORITY_ENEMYBASE);
	while (pObj != NULL)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();//タイプ取得
		switch (type)
		{
		case CObject::TYPE_ENEMY_BASE:
			//オブジェクトが敵なら
			bColl = EnemyBase(pObj, pos, rot, size, mytype, outNormal);
			break;
		default:
			break;
		}
		if (bColl == true)
		{
			return bColl;
		}	

		pObj = pObjNext;
	}	

	return bColl;
}
//=================================
// プレイヤーサイドとの当たり判定
//=================================
bool CCollision::ToPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 size, CEnemy* pEnemy)
{
	bool bColl = false;

	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = CObject::GetTop(nCnt);
		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();
			CObject::TYPE type = pObj->GetObjType();//タイプ取得
			switch (type)
			{
			case CObject::TYPE_PLAYER:
				//オブジェクトがプレイヤーなら
				bColl = Player(pObj, pos, size);
				break;
			case CObject::TYPE_BULLET:
				bColl = PlayerBullet(pObj, pos, size, pEnemy);
				break;
			default:
				break;
			}
			if (bColl == true)
			{
				return bColl;
			}

			pObj = pObjNext;
		}
	}

	return bColl;
}
//=================================
// 自販機との当たり判定
//=================================
bool CCollision::ToVender(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal)
{
	bool bColl = false;

	CObject* pObj = CObject::GetTop(PRIORITY_VENDER);
	while (pObj != NULL)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();//タイプ取得
		switch (type)
		{
		case CObject::TYPE_VENDER:
			bColl = Vender(pObj, pos,rot, size * 2.0f, mytype, outNormal);
			break;

		default:
			break;
		}
		if (bColl == true)
		{
			return bColl;
		}

		pObj = pObjNext;
	}
	
	return bColl;
}

//=================================
// ステージとの当たり判定
//=================================
bool CCollision::ToStage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal, CObject::TYPE mytype)
{
	bool bColl = false;

	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = CObject::GetTop(nCnt);
		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();
			CObject::TYPE type = pObj->GetObjType();//タイプ取得
			switch (type)
			{
			case CObject::TYPE_STAGE:
				//オブジェクトがプレイヤーなら
				bColl = Stage(pObj, pos, rot, size, outNormal);
				break;
			case CObject::TYPE_WALL:
				if (mytype != CObject::TYPE_BULLET)
				{
					bColl = InvisibleWall(pObj, pos, rot, size, outNormal);
				}
				break;

			default:
				break;
			}
			if (bColl == true)
			{
				return bColl;
			}
			pObj = pObjNext;
		}
	}

	return bColl;
}

bool CCollision::ToGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype)
{
	bool bColl = false;

	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = CObject::GetTop(nCnt);
		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();
			CObject::TYPE type = pObj->GetObjType();//タイプ取得
			switch (type)
			{
			case CObject::TYPE_GIMMICK:
				//オブジェクトがプレイヤーなら
				bColl = Gimmick(pObj, pos, size, mytype);
				break;
			case CObject::TYPE_ITEM:
				bColl = Item(pObj, pos, size, mytype);
				break;

			default:
				break;
			}
			if (bColl == true)
			{
				return bColl;
			}
			pObj = pObjNext;
		}
	}
	return bColl;
}

bool CCollision::ToStart(D3DXVECTOR3 pos, CObject::TYPE mytype)
{
	bool bColl = false;

	CObject* pObj = CObject::GetTop(PRIORITY_PLAYER);
	while (pObj != NULL)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();//タイプ取得
		switch (type)
		{
		case CObject::TYPE_PLAYER:
			//オブジェクトがプレイヤーなら
			bColl = Start(pObj, pos, mytype);
			break;
		default:
			break;
		}
		if (bColl == true)
		{
			return bColl;
		}

		pObj = pObjNext;
	}

	return bColl;
}

bool CCollision::EnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	bool bColl = false;
	CObject* pObj = CObject::GetTop(PRIORITY_PLAYER);
	while (pObj != NULL)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();//タイプ取得
		switch (type)
		{
		case CObject::TYPE_PLAYER:
		{
			CPlayer* pPlayer = (CPlayer*)pObj;
			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();//位置取得
			D3DXVECTOR3 Playersize = pPlayer->GetSize();

			bColl = Radius(pos, size, PlayerPos, Playersize);
			if (bColl == true)
			{//当たったら
				D3DXVECTOR3 move = PlayerPos - pos;
				D3DXVECTOR3 HitPos = PlayerPos + move;

				pPlayer->SetTargetPos(HitPos);
				pPlayer->Hit(1);
			}
			break;
		}
		default:
			break;
		}
		if (bColl == true)
		{
			return bColl;
		}

		pObj = pObjNext;
	}
	return bColl;
}

bool CCollision::CameraToStage(CStage* pStage)
{
	bool bColl = false;

	CCamera* pCamera = CManager::GetCamera();
	CPlayer* pPlayer = CGame::GetPlayer();

	if (!pCamera || !pPlayer || !pStage)
	{
		return false;
	}

	// 位置取得
	D3DXVECTOR3 CameraPos = pCamera->GetPosV();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 StagePos = pStage->GetPos();
	D3DXVECTOR3 StageSize = pStage->GetSize();

	// 線分情報
	D3DXVECTOR3 dir = PlayerPos - CameraPos;
	float length = D3DXVec3Length(&dir);
	if (length < 1e-6f)
	{
		return false;
	}
	D3DXVec3Normalize(&dir, &dir);

	// ステージのコライダー
	float radius = max(StageSize.x, max(StageSize.y * 1.5f, StageSize.z)) * 0.5f;

	// 線分と球の最短距離
	D3DXVECTOR3 m = StagePos - CameraPos;
	float t = D3DXVec3Dot(&m, &dir);
	t = max(0.0f, min(length, t));
	D3DXVECTOR3 closest = CameraPos + dir * t;

	D3DXVECTOR3 diff = closest - StagePos;
	float distSq = D3DXVec3LengthSq(&diff);
	if (distSq <= (radius * radius))
	{
		bColl = true;
	}
	else
	{
		bColl = false;
	}

	return bColl;
}
//=============================================================
// 
// 種類別の判定に関する処理(オブジェクトごとの当たり判定)
// 
//=============================================================
//=======================
// 敵拠点との当たり判定
//=======================
bool CCollision::EnemyBase(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal)
{
	CEnemyBase* pEnemyBase = (CEnemyBase*)pObj;
	D3DXVECTOR3 EnemyPos = pEnemyBase->GetPos();//位置取得
	D3DXVECTOR3 Enemysize = pEnemyBase->GetSize();
	D3DXVECTOR3 EnemyRot = pEnemyBase->GetRot();
	bool bColl = false;
	bool bRespawn = false;

	bColl = OBB(pos, rot, size, EnemyPos, EnemyRot, Enemysize, outNormal);
	if (bColl == true)
	{//当たったら
		if (mytype == CObject::TYPE_BULLET)
		{
			pEnemyBase->Hit(CGame::GetPlayer()->GetMyBullet());
		}
	}

	if (mytype == CObject::TYPE_PLAYER)
	{
		bRespawn = Radius(pos, size, EnemyPos, Enemysize * 8);
		pEnemyBase->SetRespawn(bRespawn);
	}

	return bColl;
}
//===============================
// 敵とプレイヤーとの判定
//===============================
bool CCollision::Player(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CPlayer* pPlayer = (CPlayer*)pObj;
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();//位置取得
	D3DXVECTOR3 Playersize = pPlayer->GetSize();

	bool bColl = Radius(pos, size, PlayerPos, Playersize);
	if (bColl == true)
	{//当たったら
		if (pPlayer->GetState() != pPlayer->STATE_HIT)
		{
			pPlayer->Hit(1);
		}
	}

	return bColl;
}
//=======================
// 敵と弾の判定
//=======================
bool CCollision::PlayerBullet(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CEnemy* pEnemy)
{
	bool bColl = false;
	CBullet* pBullet = (CBullet*)pObj;//弾取得
	if (pBullet->GetUser() == CBullet::USER_PLAYER)
	{
		D3DXVECTOR3 BulletPos = pBullet->GetPos();//位置取得
		D3DXVECTOR3 Bulletsize = pBullet->GetSize();//サイズ取得
		CBullet::TYPE BulletType = pBullet->GetType();//弾のタイプ取得
		bool bUse = pBullet->GetUse();
		//相性取得
		CMatchUp* pMatchup = CGame::GetMatchup();

		CEnemy::TYPE type = pEnemy->GetType();//敵のタイプ取得
		if (bUse == true)
		{
			//球での判定
			bColl = Radius(pos, size, BulletPos, Bulletsize);
			if (bColl == true)
			{//当たったら
				//弾と敵の相性を調べる
				int nAttack = pMatchup->Check(BulletType, type);
				//敵にダメージ
				pEnemy->Hit(nAttack);
				//弾を消す
				pBullet->SetUse(false);
			}
		}
	}
	return bColl;
}
//============================
// 自販機との当たり判定
//============================
bool CCollision::Vender(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal)
{
	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	CVender* pVender = (CVender*)pObj;

	//
	CRestock* pRestock = pVender->GetpRestock();

	D3DXVECTOR3 VenderPos = pVender->GetPos();//位置取得
	D3DXVECTOR3 VenderRot = pVender->GetRot();//位置取得
	D3DXVECTOR3 Vendersize = pVender->GetSize();

	bool bCOLL = OBB(pos, rot, size, VenderPos, VenderRot, Vendersize, outNormal);//モデル

	if (mytype == CObject::TYPE_PLAYER)
	{
		bool bColl = Cylinder(pos, size, VenderPos, Vendersize);//距離
		if (bColl == true)
		{//当たったら
			
			//中身補充
			int nRestock = pVender->GetRestock();
			//種類設定
			CBullet::TYPE type = pVender->GetType();

			if (pRestock != nullptr)
			{
				int nCntContents = pPlayer->GetContents();
				if (nCntContents <= 0 || type == pPlayer->GetMyBullet())
				{//中身０
					bool bUse = pVender->GetUse();
					if (bUse == true)
					{
						pPlayer->SetCanRestock(true);
						pVender->SetCanRestock(true);
						pRestock->Set(nRestock, type, pVender);
					}
					else
					{
					}
				}
				else
				{
				}
			}
		}
		else if (bColl == false)
		{
			pPlayer->SetCanRestock(false);
			pVender->SetCanRestock(false);
		}
	}

	if (bCOLL == true)
	{
		//ModelOn(pos, size, VenderPos, Vendersize);
	}

	return bCOLL;
}
//=========================
// ステージモデルとの判定
//=========================
bool CCollision::Stage(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal)
{
	CStage* pEnemyBase = (CStage*)pObj;
	D3DXVECTOR3 EnemyPos = pEnemyBase->GetPos();//位置取得
	D3DXVECTOR3 Enemysize = pEnemyBase->GetSize();
	D3DXVECTOR3 EnemyRot = pEnemyBase->GetRot();
	bool bColl = false;

	bColl = OBB(pos, rot, size, EnemyPos, EnemyRot, Enemysize, outNormal);

	if (bColl == true)
	{//当たったら
		
	}
	return bColl;
}
//===================
// 透明壁との判定
//===================
bool CCollision::InvisibleWall(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal)
{
	CInvisibleWall* pWall = (CInvisibleWall*)pObj;
	D3DXVECTOR3 WallPos = pWall->GetPos();//位置取得
	D3DXVECTOR3 WallRot = pWall->GetRot();//位置取得
	D3DXVECTOR3 wallSize = pWall->GetSize();

	return OBB(pos, rot, size, WallPos, WallRot, wallSize, outNormal);
}
//======================
// ギミックとの判定
//======================
bool CCollision::Gimmick(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype)
{
	CGimmick* pGimmick = (CGimmick*)pObj;
	D3DXVECTOR3 GimmickPos = pGimmick->GetPos();//位置取得
	D3DXVECTOR3 Gimmicksize = pGimmick->GetSize();
	D3DXMATRIX GimmickMtx = pGimmick->GetMtx();
	D3DXVECTOR3 GimmickRot = pGimmick->GetRot();
	bool bColl = false;

	float fRadius = max(size.x, max(size.y, size.z)) * 0.75f;
	bColl = SphereToOBB(pos, fRadius, GimmickPos, Gimmicksize, GimmickMtx);

	if (bColl == true)
	{//当たったら
		if (mytype == CObject::TYPE_PLAYER)
		{
			CPlayer* pPlayer = CGame::GetPlayer();

			if (pPlayer->GetState() != pPlayer->STATE_HIT)
			{
				CParticle::Create(GimmickPos, pPlayer->GetRot(), D3DCOLOR_RGBA(255, 1, 1, 255), 30, 5, CParticle::TYPE_NONE);
				pPlayer->Hit(3);
			}
		}
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_CAR);
	}
	return bColl;
}

bool CCollision::Start(CObject* pObj, D3DXVECTOR3 pos, CObject::TYPE mytype)
{
	bool bColl = false;

	if (mytype == CObject::TYPE_START)
	{
		CPlayer* pPlayer = (CPlayer*)pObj;
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();//位置取得
		D3DXVECTOR3 PlayerRot = pPlayer->GetRot();
		D3DXVECTOR3 Playersize = pPlayer->GetSize();
		D3DXMATRIX PlayerMtx = pPlayer->GetMtx();

		if (PlayerPos.x >= pos.x)
		{
			bColl = true;
		}

		if (bColl == true)
		{//当たったら
			pPlayer->SetPosOffSet(PlayerPos);
			pPlayer->ClearContents();

			CGame::GetMap()->GetMapPlayer()->SetUse(true);
		}
	}
	return bColl;
}

bool CCollision::Item(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype)
{
	bool bColl = false;

	if (mytype == CObject::TYPE_PLAYER)
	{
		CItem* pItem = (CItem*)pObj;//弾取得
		D3DXVECTOR3 ItemPos = pItem->GetPos();//位置取得
		D3DXVECTOR3 Itemsize = pItem->GetSize();//サイズ取得
		bool bUse = pItem->GetUse();

		if (bUse == true)
		{
			//球での判定
			bColl = Radius(pos, size, ItemPos, Itemsize);
			if (bColl == true)
			{
				pItem->Bonus();
			}
		}
	}
	return bColl;
}


//===================================================
// 
// 当たっているかを調べる処理(どう判定をとるか)
// 
//===================================================
//===================
// モデル同士の判定
//===================
bool CCollision::Model(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
{
	if (pos0.x >= pos1.x - size1.x / 2		//左
		&& pos0.x <= pos1.x + size1.x / 2	//右
		&& pos0.y + size0.y >= pos1.y		//下
		&& pos0.y <= pos1.y + size1.y		//上
		&& pos0.z >= pos1.z - size1.z / 2	//手前
		&& pos0.z <= pos1.z + size1.z / 2)	//奥
	{//当たったら
		return true;
	}

	return false;
}
//===================
// 球同士の判定
//===================
bool CCollision::Radius(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
{
	//距離
	float fDistance = (((pos1.x - pos0.x) * (pos1.x - pos0.x))
		+ ((pos1.y - pos0.y) * (pos1.y - pos0.y))
		+ ((pos1.z - pos0.z) * (pos1.z - pos0.z)));

	//それぞれの半径をサイズから算出
	float radius0 = max(size0.x, max(size0.y, size0.z)) * 0.5f;
	float radius1 = max(size1.x, max(size1.y, size1.z)) * 0.5f;

	//半径
	float RADIUS = (radius0 + radius1) * (radius0 + radius1);

	if (fDistance <= RADIUS)
	{
		return true;
	}
	return false;
}
//=============
// 壁との判定
//=============
bool CCollision::Wall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth)
{
	D3DXVECTOR3 aPos[2];
	D3DXVECTOR3 vecA;
	D3DXVECTOR3 vecB;

	// 壁の両端を算出（XZ 平面）
	aPos[0].x = pos1.x - cosf(rot1.y) * fWidth;
	aPos[0].y = 0.0f;
	aPos[0].z = pos1.z + sinf(rot1.y) * fWidth;

	aPos[1].x = pos1.x + cosf(rot1.y) * fWidth;
	aPos[1].y = 0.0f;
	aPos[1].z = pos1.z - sinf(rot1.y) * fWidth;

	D3DXVECTOR3 posXZ = pos0;
	posXZ.y = 0.0f;

	// 壁ベクトルと対象位置ベクトル（XZ平面のみ）
	vecA = aPos[1] - aPos[0];
	vecB = posXZ - aPos[0];

	// 外積で左右判定（2Dクロス積）
	float fvec = (vecA.z * vecB.x) - (vecA.x * vecB.z);

	// 投影による距離チェック
	float lenA = D3DXVec3Length(&vecA);
	float dot = D3DXVec3Dot(&vecA, &vecB);
	float projLen = dot / lenA;

	// 投影が壁の範囲内にあるか
	if (projLen < 0.0f || projLen > lenA)
	{
		return false; // 範囲外
	}

	// 左側（正面）にいるかどうか
	if (fvec < 0)
	{
		return true;
	}
	return false;
}

bool CCollision::Cylinder(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
{
	//距離
	float fDistance = (((pos1.x - pos0.x) * (pos1.x - pos0.x))
			   		 + ((pos1.z - pos0.z) * (pos1.z - pos0.z)));

	//それぞれの半径をサイズから算出
	float radius0 = max(size0.x, max(size0.y, size0.z)) * 0.5f;
	float radius1 = max(size1.x, max(size1.y, size1.z)) * 0.5f;

	//半径
	float RADIUS = (radius0 + radius1) * (radius0 + radius1);

	if (fDistance <= RADIUS)
	{
		return true;
	}
	return false;
}


//===================================================
// 
// OBB系の処理
// 
//===================================================

bool CCollision::OBB(D3DXVECTOR3 pos0, D3DXVECTOR3 rot0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, D3DXVECTOR3 size1, D3DXVECTOR3& outNormal)
{
	// ワールド行列を生成（回転と位置を合成）
	D3DXMATRIX rot0Mtx, rot1Mtx, trans0Mtx, trans1Mtx;
	D3DXMatrixRotationYawPitchRoll(&rot0Mtx, rot0.y, rot0.x, rot0.z);
	D3DXMatrixRotationYawPitchRoll(&rot1Mtx, rot1.y, rot1.x, rot1.z);
	D3DXMatrixTranslation(&trans0Mtx, pos0.x, pos0.y, pos0.z);
	D3DXMatrixTranslation(&trans1Mtx, pos1.x, pos1.y, pos1.z);

	D3DXMATRIX world0 = rot0Mtx * trans0Mtx;
	D3DXMATRIX world1 = rot1Mtx * trans1Mtx;
	
	// OBB軸ベクトルをワールド行列から取得
	D3DXVECTOR3 axes0[3] = 
	{
		D3DXVECTOR3(world0._11, world0._12, world0._13),
		D3DXVECTOR3(world0._21, world0._22, world0._23),
		D3DXVECTOR3(world0._31, world0._32, world0._33),
	};
	D3DXVECTOR3 axes1[3] = 
	{
		D3DXVECTOR3(world1._11, world1._12, world1._13),
		D3DXVECTOR3(world1._21, world1._22, world1._23),
		D3DXVECTOR3(world1._31, world1._32, world1._33),
	};

	// 軸を正規化
	for (int nCnt = 0; nCnt < 3; nCnt++) 
	{
		D3DXVec3Normalize(&axes0[nCnt], &axes0[nCnt]);
		D3DXVec3Normalize(&axes1[nCnt], &axes1[nCnt]);
	}

	// 中心同士のベクトル
	D3DXVECTOR3 T = pos1 - pos0;

	// 最小侵入量と軸を保持
	float minPenetration = FLT_MAX;
	D3DXVECTOR3 minAxis = { 0, 1, 0 }; 

	auto tryAxis = [&](const D3DXVECTOR3& axis) -> bool
	{
		// 無効な軸はスキップ
		if (D3DXVec3LengthSq(&axis) < 1e-6f)
		{
			return true;
		}

		// 軸を正規化
		D3DXVECTOR3 normAxis = axis;
		D3DXVec3Normalize(&normAxis, &normAxis);

		// 2つのOBBの投影を比較
		float fPenetration;
		if (!OverlapOnAxis(T, normAxis, axes0, size0, axes1, size1, fPenetration))
		{
			return false;
		}

		// 最小侵入軸を更新
		if (fPenetration < minPenetration)
		{
			minPenetration = fPenetration;
			minAxis = normAxis;
		}
		return true;
	};


	// 各軸チェック
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (!tryAxis(axes0[nCnt]))
		{
			return false;
		}
		if (!tryAxis(axes1[nCnt]))
		{
			return false;

		}
	}

	// 交差軸チェック
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < 3; nCnt2++)
		{
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &axes0[nCnt], &axes1[nCnt2]);
			if (!tryAxis(cross))
			{
				return false;
			}
		}
	}

	outNormal = minAxis;
	return true;
}
bool CCollision::OverlapOnAxis(const D3DXVECTOR3& T,const D3DXVECTOR3& axis,
							   const D3DXVECTOR3 axes0[3], const D3DXVECTOR3& size0,
							   const D3DXVECTOR3 axes1[3], const D3DXVECTOR3& size1, float& outPenetration)
{
	// OBB0 の半径
	float fRadius0 =
		fabsf(D3DXVec3Dot(&axis, &axes0[0])) * size0.x / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes0[1])) * size0.y / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes0[2])) * size0.z / 2;

	// OBB1 の半径
	float fRadius1 =
		fabsf(D3DXVec3Dot(&axis, &axes1[0])) * size1.x / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes1[1])) * size1.y / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes1[2])) * size1.z / 2;

	// 中心間距離
	float fDistance = fabsf(D3DXVec3Dot(&T, &axis));

	// 侵入量 
	float fPenetration = (fRadius0 + fRadius1) - fDistance;
	outPenetration = fPenetration;

	return fPenetration > 0;
}

bool CCollision::SphereToOBB(const D3DXVECTOR3& sphereCenter, float sphereRadius, const D3DXVECTOR3& obbCenter, const D3DXVECTOR3& obbSize, const D3DXMATRIX& obbRotation)
{
	// OBBの各軸
	D3DXVECTOR3 obbX(obbRotation._11, obbRotation._12, obbRotation._13);
	D3DXVECTOR3 obbY(obbRotation._21, obbRotation._22, obbRotation._23);
	D3DXVECTOR3 obbZ(obbRotation._31, obbRotation._32, obbRotation._33);

	// 球の中心からOBB中心へのベクトル
	D3DXVECTOR3 d = sphereCenter - obbCenter;

	// 最近接点を初期化（OBB空間上）
	D3DXVECTOR3 closest = obbCenter;

	// 各軸について処理
	for (int nCnt = 0; nCnt < 3; ++nCnt)
	{
		D3DXVECTOR3 axis;
		float fHalfLength = 0.0f;

		if (nCnt == 0) { axis = obbX; fHalfLength = obbSize.x / 2.0f; }
		if (nCnt == 1) { axis = obbY; fHalfLength = obbSize.y / 2.0f; }
		if (nCnt == 2) { axis = obbZ; fHalfLength = obbSize.z / 2.0f; }

		// 球中心を軸に投影
		float dist = D3DXVec3Dot(&d, &axis);

		if (dist > fHalfLength) dist = fHalfLength;
		if (dist < -fHalfLength) dist = -fHalfLength;

		// 最近接点にその軸方向を足す
		closest += axis * dist;
	}

	// 球中心と最近接点の距離を計算
	D3DXVECTOR3 diff = sphereCenter - closest;
	float distanceSq = D3DXVec3LengthSq(&diff);

	// 半径以下なら衝突
	return distanceSq <= (sphereRadius * sphereRadius);
}