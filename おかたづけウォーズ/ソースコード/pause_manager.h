//===============================================
//
// ポーズマネージャー[pause_manager.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _PAUSE_MANAGER_H_
#define _PAUSE_MANAGER_H_

// インクルード
#include "main.h"
#include "object.h"
#include "pause_select.h"

// ポーズマネージャークラス
class CPauseMana
{
public:
	// 選択肢
	typedef enum
	{
		SELECT_CONTNUE = 0,
		SELECT_RESTART,
		SELECT_END,
		SELECT_MAX
	}SELECT;

	// メンバ関数
	CPauseMana();
	~CPauseMana();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	// 設定処理
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

	// 静的メンバ関数
	static CPauseMana* Create(D3DXVECTOR3 pos);

private:	
	void Select(void);
	
	// 定数定義
	static constexpr float P_SELECT_X = 250.0f;	// 選択肢のXサイズ
	static constexpr float P_SELECT_Y = 100.0f;	// 選択肢のYサイズ

	// メンバ変数
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 角度
	CPauseSelect* m_pSelect[SELECT_MAX];	// ポーズ選択へのポインタ
	CObject2D* m_pBack;						// 背景へのポインタ
	int m_Select;							// 選択しているもの
	bool m_bDraw;							// 描画するかどうか
};

#endif