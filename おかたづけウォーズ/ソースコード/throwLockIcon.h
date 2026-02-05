//===============================================
//
// 投げられない時用アイコン[throwLockIcon.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _THROWLOCKICON_H_
#define _THROWLOCKICON_H_

// インクルード
#include "main.h"
#include "objectBillboard.h"

// 投げられない時用アイコンクラス
class CThrowLockIcon :public CObjectBillboard
{
public:

	// メンバ関数
	CThrowLockIcon(int nPriority = static_cast<int>(Priority::UI));
	~CThrowLockIcon();

	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(150.0f, 50.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetColor(D3DXCOLOR col) { m_col = col; m_bUse = true; }
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CThrowLockIcon* Create(const char* pTexName, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(200.0f, 100.0f));

private:

	// メンバ変数
	D3DXCOLOR m_col;	// 色
	bool m_bUse;		// 使用しているか
};

#endif