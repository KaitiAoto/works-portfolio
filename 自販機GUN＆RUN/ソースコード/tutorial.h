//==============================
//
// チュートリアル[tutorial.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// インクルード
#include "main.h"
#include "object2D.h"

// マクロ定義
#define TUTORIAL_SIZE (250)

// チュートリアルクラス
class CTutorial:public CObject2D
{
public:
	// 種類
	typedef enum
	{
		TYPE_MOVE = 0,
		TYPE_CAMERA,
		TYPE_BULLET,
		TYPE_RESTOCK,
		TYPE_CHANGE,
		TYPE_HOLD,
		TYPE_CONTENTS,
		TYPE_MATCHUP,
		TYPE_MAX
	}TYPE;
	// 状態
	typedef enum
	{
		STATE_STAY = 0,
		STATE_SLIDEIN,
		STATE_SLIDEOUT,
		STATE_MAX
	}STATE;


	// メンバ関数
	CTutorial(int nPriority = 9);
	~CTutorial();

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetUse(void){return m_bUse;}
	TYPE GetType(void) { return m_type; }
	// 設定処理
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetClear(bool bClear) { m_bClear = bClear; }

	// 静的メンバ関数
	static CTutorial* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	void BackBlink(void);
	void CheckClear(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posStay;				// 止まる位置
	D3DXVECTOR3 m_posOut;				// 画面外位置
	D3DXVECTOR3 m_rot;					// 角度
	D3DXCOLOR m_col;					// 色
	TYPE m_type;						// 種類
	STATE m_state;						// 状態
	CObject2D* m_pBack;					// 背景へのポインタ
	CObject2D* m_pSkip;					// スキップＵＩへのポインタ
	const char* m_apFileName[TYPE_MAX];	// テクスチャ名
	bool m_bUse;						// 使用しているか
	bool m_bClear;						// クリアしているか
	int m_nIdxTex;						// テクスチャのインデックス番号
	int m_nTime;						// タイマー
	int m_nCntTime;						// タイマーカウント

};

#endif