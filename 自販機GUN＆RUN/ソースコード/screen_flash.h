//=====================================
//
// 画面フラッシュ[screen_flash.h]
// Author : Kaiti Aoto
//
//=====================================

// 二重インクルード防止
#ifndef _SCREEN_FLASH_H_
#define _SCREEN_FLASH_H_

// インクルード
#include "main.h"
#include "object2D.h"

// 画面フラッシュクラス
class CScreenFlash:public CObject2D
{
public:

	// メンバ関数
	CScreenFlash(int nPriority = 9);
	~CScreenFlash();

	HRESULT Init(const char* pTexName, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	
	// 設定処理
	void SetColor(D3DXCOLOR col) { m_col = col; m_bUse = true; }

	// 静的メンバ関数
	static CScreenFlash* Create(const char* pTexName, D3DXCOLOR col);

private:

	// メンバ変数
	D3DXCOLOR m_col;	// 色
	bool m_bUse;		// 使用しているか
};

#endif