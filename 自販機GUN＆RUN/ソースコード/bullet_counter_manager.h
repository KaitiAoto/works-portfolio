//================================================================
//
// 弾数カウンターマネージャー[bullet_counter_manager.h]
// Author : Kaiti Aoto
//
//================================================================

// 二重インクルード防止
#ifndef _BULLET_COUNTER_MANAGER_H_
#define _BULLET_COUNTER_MANAGER_H_

// インクルード
#include "main.h"
#include "bullet_counter.h"
#include "bullet_icon.h"

// マクロ定義
#define MAX_BULLETCNT (2)
#define BULLETCNTDATA (10)

// 弾数カウンターマネージャークラス
class CBulletCntMana
{
public:

	//メンバ関数
	CBulletCntMana();
	~CBulletCntMana();

	HRESULT Init(D3DXVECTOR3 pos, bool bSub);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateCounter(void);
	void ResetCnt(void) { m_nCount = 0; }
	void BecomeMain();
	void BecomeSub();

	void SetCnt(int nCnt) { m_nCount = nCnt; }
	void SetType(CBullet::TYPE type) { m_Bullet = type; }
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	int GetContents(void) { return m_nCount; }
	CBullet::TYPE GetType(void) { return m_Bullet; }

	// 静的メンバ関数
	static CBulletCntMana* Create(D3DXVECTOR3 pos, bool bSub);

private:
	//メンバ変数
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	int m_nCount;								// カウント
	CBullerCounter* m_pCounter[MAX_BULLETCNT];	// カウンターへのポインタ
	CBulletIcon* m_pIcon;						// 弾アイコンへのポインタ
	CBullet::TYPE m_Bullet;				// 弾の種類
	CObject2D* m_cross;
	float m_fIconSize;
	float m_fCntSize;

	//静的メンバ変数
	static D3DXVECTOR3 m_MainPos;
	static D3DXVECTOR3 m_SubPos;

};

#endif