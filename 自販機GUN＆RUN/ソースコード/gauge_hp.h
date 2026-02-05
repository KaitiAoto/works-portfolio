//==============================
//
//  HPゲージ処理[gauge_hp.h]
//  Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _GAUGE_HP_H_
#define _GAUGE_HP_H_

// インクルード
#include"main.h"
#include "gauge.h"

// HPゲージクラス
class CHpGauge :CObject
{
public:
	// メンバ関数
	CHpGauge(int nPriority = 8);
	~CHpGauge();
	HRESULT Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CHpGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXCOLOR m_col;	// 色
	float m_Base;		// 基準
	float m_fHeight;	// 高さ
	float m_rate;		// レート

	CGauge* m_pGauge;	// ゲージへのポインタ
};
#endif
