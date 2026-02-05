//===============================================
//
// 当たり判定[collision.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _COLLISION_H_
#define _COLLISION_H_

// インクルード
#include "damegeArea.h"
#include "interact.h"

// 当たり判定クラス
class CColl
{
public:

	// メンバ関数
	CColl();
	~CColl();
	bool Radius(D3DXVECTOR3 pos0, float fRadius0, D3DXVECTOR3 pos1, float fRadius1);
	bool OBB(const Obb& obb0, const Obb& obb1);
private:

};


#endif