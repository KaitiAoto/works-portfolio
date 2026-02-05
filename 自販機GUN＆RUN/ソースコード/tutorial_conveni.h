//==============================
//
// チュートリアル[tutorial.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _TUTORIAL_CONVENI_H_
#define _TUTORIAL_CONVENI_H_

// インクルード
#include "main.h"
#include "object2D.h"
#include "click.h"

// マクロ定義

// チュートリアルクラス
class CTutorialConveni:public CObject2D
{
public:
	// 種類
	typedef enum
	{
		TYPE_STOCK = 0,
		TYPE_RESTOCK,
		TYPE_ENEMY,
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
	CTutorialConveni(int nPriority = 9);
	~CTutorialConveni();

	HRESULT Init();
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
	static CTutorialConveni* Create();

private:
	void CheckClear(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posStay;				// 止まる位置
	D3DXVECTOR3 m_posOut;				// 画面外位置
	D3DXVECTOR3 m_rot;					// 角度
	D3DXCOLOR m_col;					// 色
	TYPE m_type;						// 種類
	STATE m_state;						// 状態
	CObject2D* m_pSkip;					// スキップＵＩへのポインタ
	CClick* m_pClick;					// 決定キーＵＩへのポインタ
	const char* m_apFileName[TYPE_MAX];	// テクスチャ名
	bool m_bUse;						// 使用しているか
	bool m_bClear;						// クリアしているか
	int m_nIdxTex;						// テクスチャのインデックス番号
	float m_fWidth;
	float m_fHeight;
};

#endif