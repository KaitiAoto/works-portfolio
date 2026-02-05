//=====================================
//
// 弾数カウンター[bullet_counter.h]
// Author : Kaiti Aoto
//
//=====================================

// 二重インクルード防止
#ifndef _BULLET_COUNTER_SYSTEM_H_
#define _BULLET_COUNTER_SYSTEM_H_

#include "main.h"
#include "object.h"
#include "bullet_counter_manager.h"

// マクロ定義

// 弾数カウンタークラス
class CBullerCntSystem :public CObject
{
public:
	
	// メンバ関数
	CBullerCntSystem(int nPriority = 8);
	~CBullerCntSystem();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Change(void);
	void Reset(void);
	

	void SetbMove(bool bMove) { m_bMove = bMove; }
	void SetBlink(bool bBlink) { m_bBlink = bBlink; }

	CBulletCntMana* GetUseCnt(void);

	// 静的メンバ関数
	static CBullerCntSystem* Create(void);


private:
	void Move(void);
	void Blink(void);

	// メンバ変数
	D3DXVECTOR3 m_MainPos;
	D3DXVECTOR3 m_SubPos;
	D3DXVECTOR3 m_rot;

	CBulletCntMana* m_pBulletCount;			// 弾数カウンターへのポインタ
	CBulletCntMana* m_pSubBulletCount;		// 弾数カウンターへのポインタ

	bool m_bWhich;
	bool m_bMove;
	bool m_bBlink;
	bool m_bRLShake;

	int m_nCntBlink;
};

#endif