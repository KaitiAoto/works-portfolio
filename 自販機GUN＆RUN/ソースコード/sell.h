//==============================
//
// 相性[matchup.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _SELL_H_
#define _SELL_H_

// インクルード
#include "main.h"
#include "enemy_base.h"

// マクロ定義

// 相性クラス
class CSell
{
public:
	// メンバ関数
	CSell();
	~CSell();

	void Init(void);
	static void Update(void);

private:
	
	// メンバ変数
	static int m_nCntSellTime;
	static int m_nCntSell;

};

#endif