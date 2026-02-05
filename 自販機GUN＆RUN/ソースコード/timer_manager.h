//===========================================
//
// タイマーマネージャー[timer_manager.h]
// Author : Kaiti Aoto
//
//===========================================

// 二重インクルード防止
#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_

// インクルード
#include "main.h"
#include "timer.h"

// マクロ定義
#define TIMERDATA (10)
#define TIME_DIGIT (2)

// タイマーマネージャークラス
class CTimerMana
{
public:

	// メンバ関数
	CTimerMana();
	~CTimerMana();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTimerMana* Create(D3DXVECTOR3 pos);
	static void AddTime(int nAdd);
	static int GetTime(void) { return m_nTimer; }
private:
	static void Second(void);
	static void Minute(void);
	static void CntDown(void);

	// 静的メンバ変数
	static CTimer* m_pSecond[TIME_DIGIT];	// 秒数へのポインタ
	static CTimer* m_pMinute[TIME_DIGIT];	// 分数へのポインタ
	static int m_nTimer;					// 現在の時間
	static int m_nCntTime;					// 1秒計測用
	static int m_nDrawSecond;				// 描画する秒数
	static int m_nDrawMinute;				// 描画する分数

	static float m_fWidth;
	static float m_fHeight;

};

#endif