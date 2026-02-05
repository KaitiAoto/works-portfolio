//===============================================
//
// ポーズ選択[pause_select.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _PAUSE_SELECT_H_
#define _PAUSE_SELECT_H_

// インクルード
#include "main.h"
#include "object2D.h"

// ポーズ選択クラス
class CPauseSelect:public CObject2D
{
public:

	// メンバ関数
	CPauseSelect(int nPriority = static_cast<int>(Priority::Pause));
	~CPauseSelect();
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

	// 静的メンバ関数
	static CPauseSelect* Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	//メンバ変数
	bool m_bDraw;	// 描画するかどうか

};

#endif