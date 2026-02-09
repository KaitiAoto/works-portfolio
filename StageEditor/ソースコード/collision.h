//==============================
//
// 当たり判定[collision.h]
// Author:kaiti
//
//==============================
#ifndef COLLISION_H_
#define COLLISION_H_

#include "main.h"
#include "object.h"

//オブジェクトクラス
class CCollision
{
public:
	//メンバ関数
	CCollision();
	~CCollision();

	bool CollisionToStage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXMATRIX mtx);
private:
	bool CollisionModel(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);
	void CollisionModelOn(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);
	bool CollisionRadius(D3DXVECTOR3 pos0, D3DXVECTOR3 size0, D3DXVECTOR3 pos1, D3DXVECTOR3 size1);

	bool CollisionPlayer(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	bool CollisionStage(CObject* pObj, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXMATRIX mtx);

	bool CollisionOBB(D3DXVECTOR3 pos0, D3DXVECTOR3 rot0, D3DXVECTOR3 size0, D3DXMATRIX mtx0, D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, D3DXVECTOR3 size1, D3DXMATRIX mtx1);
	bool OverlapOnAxis(const D3DXVECTOR3& T, const D3DXVECTOR3& axis,
                              const D3DXVECTOR3 axes0[3], const D3DXVECTOR3& size0,
                              const D3DXVECTOR3 axes1[3], const D3DXVECTOR3& size1);
	//メンバ変数

};

#endif