//==============================
//
// 当たり判定[collision.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _COLLISION_H_
#define _COLLISION_H_

// インクルード
#include "main.h"
#include "object.h"
#include "enemy.h"
#include "stage.h"

// 当たり判定クラス
class CCollision
{
public:
	// メンバ関数
	CCollision();
	~CCollision();

	bool ToEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal);
	bool ToPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 size, CEnemy* pEnemy);
	bool ToVender(D3DXVECTOR3 pos, D3DXVECTOR3 rot , D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal);
	bool ToStage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal, CObject::TYPE mytype);
	bool ToGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype);
	bool ToStart(D3DXVECTOR3 pos, CObject::TYPE mytype);
	bool EnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	bool CameraToStage(CStage* pStage);

	bool OverlapOnAxis(const D3DXVECTOR3& T, const D3DXVECTOR3& axis,
		const D3DXVECTOR3 axes0[3], const D3DXVECTOR3& size0,
		const D3DXVECTOR3 axes1[3], const D3DXVECTOR3& size1, float& outPenetration);

private:
	bool Model(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);
	bool Radius(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);
	bool Wall(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth);
	bool Cylinder(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);

	bool EnemyBase(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal);
	bool Player(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	bool PlayerBullet(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CEnemy* pEnemy);
	bool Vender(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, CObject::TYPE mytype, D3DXVECTOR3& outNormal);
	bool Stage(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal);
	bool InvisibleWall(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXVECTOR3& outNormal);
	bool Gimmick(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype);
	bool Start(CObject* pObj, D3DXVECTOR3 pos, CObject::TYPE mytype);
	bool Item(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size, CObject::TYPE mytype);

	bool OBB(D3DXVECTOR3 pos0, D3DXVECTOR3 rot0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, D3DXVECTOR3 size1, D3DXVECTOR3& outNormal);
	bool SphereToOBB(const D3DXVECTOR3& sphereCenter, float sphereRadius, const D3DXVECTOR3& obbCenter, const D3DXVECTOR3& obbSize, const D3DXMATRIX& obbRotation);

	//メンバ変数

};

#endif