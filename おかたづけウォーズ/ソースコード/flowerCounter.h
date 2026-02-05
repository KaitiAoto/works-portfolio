//===============================================
//
// 花数カウンター[flowerCounter.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _FLOWERCOUNTER_H_
#define _FLOWERCOUNTER_H_

// インクルード
#include "main.h"
#include "number.h"
#include "object.h"

// 前方宣言
class CObjectX;

// 花数カウンタークラス
class CFlowerCnt :public CObject
{
public:

	//メンバ関数
	CFlowerCnt(int nPriority = static_cast<int>(Priority::StageUI));
	~CFlowerCnt();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddHaveFlower(void);
	void ResetHaveFlower(void);
	
	// 設定
	void SetShakeInfo(bool bShake, int nShakeTime, D3DXVECTOR3 MaxShake);

	// 静的メンバ関数
	static CFlowerCnt* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static int GetCreateBouqet(void) { return CREATE_BOUQET; }
private:

	static constexpr int CREATE_BOUQET = 5;	// 花束形成に必要な個数

	//メンバ変数
	CObjectX* m_pCircle[CREATE_BOUQET];	// 〇へのポインタ
	CObjectX* m_pFlower[CREATE_BOUQET];	// アイコンへのポインタ
	int m_nCntFlower;					// 持っている花の数
};

#endif