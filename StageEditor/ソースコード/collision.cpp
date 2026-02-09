//==============================
//
// 当たり判定[collision.cpp]
// Author:kaiti
//
//==============================
#include "collision.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"
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

//========================================================================
// 
// 判定をとる相手の種類を調べる処理(どんなオブジェクトとの判定をとるか)
// 
//========================================================================
bool CCollision::CollisionToStage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXMATRIX mtx)
{


	return false;
}


//===================================================
// 
// 種類別の判定に関する処理(何との判定をとるか)
// 
//===================================================

bool CCollision::CollisionStage(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXMATRIX mtx)
{


	return false;
}


//===================================================
// 
// 当たっているかを調べる処理(どう判定をとるか)
// 
//===================================================
//===================
// モデル同士の判定
//===================
bool CCollision::CollisionModel(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
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
//========================
// プレイヤーの着地判定
//========================
void CCollision::CollisionModelOn(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
{
	//プレイヤー情報取得
	CPlayer* pPlayer = CManager::GetPlayer();
	D3DXVECTOR3 posOld = pPlayer->GetPosOld();
	bool bLanding = false;
	if (posOld.y >= pos1.y + size1.y
		&& pos0.y < pos1.y + size1.y)
	{//ブロックに上からめり込んだ時
		bLanding = true;
	}
	else
	{
	}
	pPlayer->SetLanding(bLanding);
}
//===================
// 球同士の判定
//===================
bool CCollision::CollisionRadius(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1)
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


//===================================================
// 
// OBB系の処理
// 
//===================================================

bool CCollision::CollisionOBB(D3DXVECTOR3 pos0, D3DXVECTOR3 rot0, D3DXVECTOR3 size0, D3DXMATRIX mtx0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, D3DXVECTOR3 size1, D3DXMATRIX mtx1)
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
	D3DXVECTOR3 axes0[3] = {
		D3DXVECTOR3(world0._11, world0._12, world0._13),
		D3DXVECTOR3(world0._21, world0._22, world0._23),
		D3DXVECTOR3(world0._31, world0._32, world0._33),
	};
	D3DXVECTOR3 axes1[3] = {
		D3DXVECTOR3(world1._11, world1._12, world1._13),
		D3DXVECTOR3(world1._21, world1._22, world1._23),
		D3DXVECTOR3(world1._31, world1._32, world1._33),
	};

	for (int i = 0; i < 3; i++) {
		D3DXVec3Normalize(&axes0[i], &axes0[i]);
		D3DXVec3Normalize(&axes1[i], &axes1[i]);
	}

	D3DXVECTOR3 T = pos1 - pos0;

	for (int i = 0; i < 3; i++) {
		if (!OverlapOnAxis(T, axes0[i], axes0, size0, axes1, size1)) return false;
		if (!OverlapOnAxis(T, axes1[i], axes0, size0, axes1, size1)) return false;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			D3DXVECTOR3 axis;
			D3DXVec3Cross(&axis, &axes0[i], &axes1[j]);
			if (D3DXVec3LengthSq(&axis) < 1e-6f) continue;
			D3DXVec3Normalize(&axis, &axis);
			if (!OverlapOnAxis(T, axis, axes0, size0, axes1, size1)) return false;
		}
	}

	return true;
}
bool CCollision::OverlapOnAxis(const D3DXVECTOR3& T,const D3DXVECTOR3& axis,
							   const D3DXVECTOR3 axes0[3], const D3DXVECTOR3& size0,
							   const D3DXVECTOR3 axes1[3], const D3DXVECTOR3& size1)
{
	float r0 =
		fabsf(D3DXVec3Dot(&axis, &axes0[0])) * size0.x / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes0[1])) * size0.y / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes0[2])) * size0.z / 2;

	float r1 =
		fabsf(D3DXVec3Dot(&axis, &axes1[0])) * size1.x / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes1[1])) * size1.y / 2 +
		fabsf(D3DXVec3Dot(&axis, &axes1[2])) * size1.z / 2;

	float distance = fabsf(D3DXVec3Dot(&T, &axis));

	return distance <= (r0 + r1);
}