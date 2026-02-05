//===============================================
//
// 当たり判定[collision.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "collision.h"
#include "player.h"
#include "game.h"

//===============================================
// コンストラクタ
//===============================================
CColl::CColl()
{

}
//===============================================
// デストラクタ
//===============================================
CColl::~CColl()
{

}

//===============================================
// 半径同士の判定
//===============================================
bool CColl::Radius(D3DXVECTOR3 pos0, float fRadius0, D3DXVECTOR3 pos1, float fRadius1)
{
	// 距離
	float fDistance = (((pos0.x - pos1.x) * (pos0.x - pos1.x))
					 + ((pos0.y - pos1.y) * (pos0.y - pos1.y))
					 + ((pos0.z - pos1.z) * (pos0.z - pos1.z)));

	// 半径
	float RADIUS = (fRadius1 + fRadius0) * (fRadius1 + fRadius0);

	// 距離が半径より小さければ
	if (fDistance <= RADIUS)
	{
		return true;
	}

	return false;
}
//===============================================
// OBB同士の判定
//===============================================
bool CColl::OBB(const Obb& obb0, const Obb& obb1)
{
    // 各軸のローカル軸
    D3DXVECTOR3 axis0[3] = { obb0.axis[0], obb0.axis[1], obb0.axis[2] };
    D3DXVECTOR3 axis1[3] = { obb1.axis[0], obb1.axis[1], obb1.axis[2] };

    // 中心点の差
    D3DXVECTOR3 t = obb1.centerPos - obb0.centerPos;
    // ローカル座標系に変換
    t = D3DXVECTOR3(D3DXVec3Dot(&t, &axis0[0]),
        D3DXVec3Dot(&t, &axis0[1]),
        D3DXVec3Dot(&t, &axis0[2]));

    // 回転行列と絶対値行列
    float R[3][3];
    float AbsR[3][3];
    const float EPSILON = 1e-5f;

    // 各軸間のドット積を計算
    for (int nCnt = 0; nCnt < 3; nCnt++)
    {
        for (int nCnt1 = 0; nCnt1 < 3; nCnt1++)
        {
            R[nCnt][nCnt1] = D3DXVec3Dot(&axis0[nCnt], &axis1[nCnt1]);
            AbsR[nCnt][nCnt1] = fabs(R[nCnt][nCnt1]) + EPSILON;
        }
    }

    // 各軸ごとに分離軸があるかをテスト
    float ra, rb;

    // Aの軸3本
    for (int nCnt = 0; nCnt < 3; nCnt++)
    {
        ra = obb0.size[nCnt];
        rb = obb1.size.x * AbsR[nCnt][0] + obb1.size.y * AbsR[nCnt][1] + obb1.size.z * AbsR[nCnt][2];
        if (fabs((&t.x)[nCnt]) > ra + rb) return false;
    }

    // Bの軸3本
    for (int nCnt = 0; nCnt < 3; nCnt++)
    {
        ra = obb0.size.x * AbsR[0][nCnt] + obb0.size.y * AbsR[1][nCnt] + obb0.size.z * AbsR[2][nCnt];
        rb = obb1.size[nCnt];
        float tval = fabs(t.x * R[0][nCnt] + t.y * R[1][nCnt] + t.z * R[2][nCnt]);
        if (tval > ra + rb) return false;
    }

    // 交差軸（A×B）9本
    // 各A軸×各B軸の外積軸をチェック
    // A0 x B0
    ra = obb0.size.y * AbsR[2][0] + obb0.size.z * AbsR[1][0];
    rb = obb1.size.y * AbsR[0][2] + obb1.size.z * AbsR[0][1];
    if (fabs(t.z * R[1][0] - t.y * R[2][0]) > ra + rb) return false;

    // A0 x B1
    ra = obb0.size.y * AbsR[2][1] + obb0.size.z * AbsR[1][1];
    rb = obb1.size.x * AbsR[0][2] + obb1.size.z * AbsR[0][0];
    if (fabs(t.z * R[1][1] - t.y * R[2][1]) > ra + rb) return false;

    // A0 x B2
    ra = obb0.size.y * AbsR[2][2] + obb0.size.z * AbsR[1][2];
    rb = obb1.size.x * AbsR[0][1] + obb1.size.y * AbsR[0][0];
    if (fabs(t.z * R[1][2] - t.y * R[2][2]) > ra + rb) return false;

    // A1 x B0
    ra = obb0.size.x * AbsR[2][0] + obb0.size.z * AbsR[0][0];
    rb = obb1.size.y * AbsR[1][2] + obb1.size.z * AbsR[1][1];
    if (fabs(t.x * R[2][0] - t.z * R[0][0]) > ra + rb) return false;

    // A1 x B1
    ra = obb0.size.x * AbsR[2][1] + obb0.size.z * AbsR[0][1];
    rb = obb1.size.x * AbsR[1][2] + obb1.size.z * AbsR[1][0];
    if (fabs(t.x * R[2][1] - t.z * R[0][1]) > ra + rb) return false;

    // A1 x B2
    ra = obb0.size.x * AbsR[2][2] + obb0.size.z * AbsR[0][2];
    rb = obb1.size.x * AbsR[1][1] + obb1.size.y * AbsR[1][0];
    if (fabs(t.x * R[2][2] - t.z * R[0][2]) > ra + rb) return false;

    // A2 x B0
    ra = obb0.size.x * AbsR[1][0] + obb0.size.y * AbsR[0][0];
    rb = obb1.size.y * AbsR[2][2] + obb1.size.z * AbsR[2][1];
    if (fabs(t.y * R[0][0] - t.x * R[1][0]) > ra + rb) return false;

    // A2 x B1
    ra = obb0.size.x * AbsR[1][1] + obb0.size.y * AbsR[0][1];
    rb = obb1.size.x * AbsR[2][2] + obb1.size.z * AbsR[2][0];
    if (fabs(t.y * R[0][1] - t.x * R[1][1]) > ra + rb) return false;

    // A2 x B2
    ra = obb0.size.x * AbsR[1][2] + obb0.size.y * AbsR[0][2];
    rb = obb1.size.x * AbsR[2][1] + obb1.size.y * AbsR[2][0];
    if (fabs(t.y * R[0][2] - t.x * R[1][2]) > ra + rb) return false;

    // すべての軸で分離がなければ交差
    return true;
}
