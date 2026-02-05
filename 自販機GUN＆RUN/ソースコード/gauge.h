//==============================
//
//  ゲージ処理[Gauge.h]
//  Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _GAUGE_H_
#define _GAUGE_H_

// インクルード
#include"main.h"
#include "object.h"

// マクロ定義
#define GAUGE_X (20)
#define GAUGE_Y (20)

// ゲージクラス
class CGauge
{
public:

	// メンバ関数
	CGauge();
	~CGauge();
	HRESULT Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetBase(float base) { m_Base = base; }
	void SetRate(float rate) { m_rate = rate; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void Set();

	// 静的メンバ関数
	static CGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXCOLOR m_col;					// 色
	float m_Base;						// 基準
	float m_fHeight;					// 高さ
	float m_rate;						// レート
	int m_nIdxTex;						// テクスチャのインデックス番号

};
#endif
