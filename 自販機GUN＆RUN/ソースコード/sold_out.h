//==============================
//
// 売り切れ表示[sold_out.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _SOLD_OUT_H_
#define _SOLD_OUT_H_

// インクルード
#include "main.h"
#include "object2D.h"

// 売り切れ表示クラス
class CSoldOut:public CObject2D
{
public:

	// メンバ関数
	CSoldOut(int nPriority = 9);
	~CSoldOut();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	static bool GetUse(void) { return m_bUse; }

	// 設定処理
	void SetColor(D3DXCOLOR col) { m_col = col; m_bUse = true; }

	// 静的メンバ関数
	static CSoldOut* Create(void);
private:

	// メンバ変数
	D3DXCOLOR m_col;	// 色

	// 静的メンバ変数
	static bool m_bUse;	// 使用しているか
};

#endif