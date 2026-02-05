//==============================
//
// 決定キーUI[click.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _CLICK_H_
#define _CLICK_H_

// インクルード
#include "main.h"
#include "object2D.h"

// マクロ定義
#define CLICK_SIZE_X (250)
#define CLICK_SIZE_Y (50)

// 決定キーUIクラス
class CClick:public CObject2D
{
public:
	//メンバ関数
	CClick(int nPriority = 10);
	~CClick();

	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	//設定処理
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CClick* Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	void Blink(void);
	//メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	bool m_bUse;		// 有効か無効か
	int m_nCntTime;		// カウンター
};

#endif