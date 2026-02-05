//==============================
//
// 開始ＵＩ[startUI.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _STARTUI_H_
#define _STARTUI_H_

// インクルード
#include "main.h"
#include "object2D.h"

// マクロ定義
#define STARTUI_SIZE_X (600)
#define STARTUI_SIZE_Y (300)

// 開始ＵＩクラス
class CStartUI:public CObject2D
{
public:

	// メンバ関数
	CStartUI(int nPriority = 7);
	~CStartUI();

	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	// 設定処理

	// 静的メンバ関数
	static CStartUI* Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	void Move(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posStay;	// 位置(止まる場所)
	D3DXVECTOR3 m_rot;		// 角度
	bool m_bStop;			// 止まっているかどうか
	bool m_bUse;			// 使用しているか
	int m_nTime;			// 止まっている時間
};

#endif