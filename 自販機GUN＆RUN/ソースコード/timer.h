//=============================
//
// タイマー[timer.h]
// Author : Kaiti Aoto
//
//=============================

// 二重インクルード防止
#ifndef _TIMER_H_
#define _TIMER_H_

// インクルード
#include "main.h"
#include "number.h"
#include "object.h"

// マクロ定義
#define TIMER_SIZE (25)

// タイマークラス
class CTimer:public CObject
{
public:

	// メンバ関数
	CTimer(int nPriority = 8);
	~CTimer();
	
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	CNumber* GetNumber(void){return m_pNumber;}

	// 静的メンバ関数
	static CTimer* Create(D3DXVECTOR3 pos);
private:
	// メンバ変数
	CNumber* m_pNumber;		// 数字へのポインタ
	float m_fWidth;
	float m_fHeight;
};

#endif