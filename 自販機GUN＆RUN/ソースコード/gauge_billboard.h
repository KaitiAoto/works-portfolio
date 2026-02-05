//============================================
//
//  ビルボードゲージ処理[gauge_billboard.h]
//  Author : Kaiti Aoto
//
//============================================

// 二重インクルード防止
#ifndef _GAUGE_BILLBOARD_H_
#define _GAUGE_BILLBOARD_H_

// インクルード
#include"main.h"
#include "object.h"

// ビルボードゲージクラス
class CGaugeBillboard
{
public:

	// メンバ関数
	CGaugeBillboard();
	~CGaugeBillboard();
	HRESULT Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void Set();
	void SetBase(float base) { m_Base = base; }
	void SetRate(float rate) { m_rate = rate; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	// 静的メンバ関数
	static CGaugeBillboard* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// メンバ変数
	int m_nIdxTex;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXCOLOR m_col;					// 色
	float m_Base;						// 基準
	float m_fHeight;					// 高さ
	float m_fWidth;						// 横幅
	float m_rate;						// レート
};
#endif
