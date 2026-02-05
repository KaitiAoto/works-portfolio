//===================================
//
//  敵HPゲージ処理[gauge_enemy.h]
//  Author : Kaiti Aoto
//
//===================================

// 二重インクルード防止
#ifndef _GAUGE_ENEMY_H_
#define _GAUGE_ENEMY_H_

// インクルード
#include "main.h"
#include "gauge_billboard.h"

// 敵HPゲージクラス
class CEnemyGauge :CObject
{
public:

	// メンバ関数
	CEnemyGauge(int nPriority = 7);
	~CEnemyGauge();
	HRESULT Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 設定
	void SetBase(float nBase) { m_Base = nBase; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	void SetRate(float rate) { m_rate = rate; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	// 静的メンバ関数
	static CEnemyGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// メンバ変数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXCOLOR m_col;			// 色
	float m_Base;				// 基準
	float m_fHeight;			// 高さ
	float m_rate;				// レート
	CGaugeBillboard* m_pGauge;	// ビルボードゲージへのポインタ
	bool m_bDraw;				// 描画するか
};
#endif
