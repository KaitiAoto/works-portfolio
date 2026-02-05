//===================================
//
// タイトル選択[title_select.h]
// Author : Kaiti Aoto
//
//====================================

// 二重インクルード防止
#ifndef _TITLE_SELECT_H_
#define _TITLE_SELECT_H_

// インクルード
#include "main.h"
#include "object.h"
#include "object2D.h"

// マクロ定義
#define T_SELECT_X (250)
#define T_SELECT_Y (100)

// タイトル選択クラス
class CTitleSelect:public CObject
{
public:
	// 選択肢
	typedef enum
	{
		SELECT_GAME = 0,
		SELECT_END,
		SELECT_MAX
	}SELECT;


	// メンバ関数
	CTitleSelect(int nPriority = 7);
	~CTitleSelect();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	// 設定処理

	// 静的メンバ関数
	static CTitleSelect* Create(D3DXVECTOR3 pos);

private:
	void Select(void);
	
	// メンバ変数
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 角度
	CObject2D* m_pSelect[SELECT_MAX];	// 選択肢へのポインタ
	CObject2D* m_pCursor;				// 選択カーソルへのポインタ
	int m_Select;						// どれを選択しているか
	float m_fAnim[SELECT_MAX];			// アニメカウント
};

#endif