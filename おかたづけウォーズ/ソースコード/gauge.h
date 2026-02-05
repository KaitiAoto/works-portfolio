//===============================================
//
//  ゲージ処理[Gauge.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GAUGE_H_
#define _GAUGE_H_

// インクルード
#include"main.h"
#include "object.h"

// ゲージクラス
class CGauge :public CObject
{
public:

	// メンバ関数
	CGauge(int nPriority = static_cast<int>(Priority::UI_Front));
	~CGauge();
	HRESULT Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(20.0f, 20.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateFrame(const char* pFileName);
	void CreateBack(const char* pFileName, D3DXCOLOR col);

	// 設定
	void Set(float fBase, D3DXCOLOR col);

	// 静的メンバ関数
	static CGauge* Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(20.0f, 20.0f));

private:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR2 m_size;					// サイズ
	D3DXCOLOR m_col;					// 色
	float m_Base;						// 基準
	float m_MaxBase;					// 最大基準
	int m_nIdxTex;						// テクスチャのインデックス番号

};
#endif
