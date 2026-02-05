//===============================================
//
//  ビルボードゲージ処理[gauge_billboard.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GAUGE_BILLBOARD_H_
#define _GAUGE_BILLBOARD_H_

// インクルード
#include"main.h"
#include "object.h"

// ビルボードゲージクラス
class CGaugeBillboard :public CObject
{
public:

	// メンバ関数
	CGaugeBillboard(int nPriority = static_cast<int>(Priority::StageUI));
	~CGaugeBillboard();
	HRESULT Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 50.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateFrame(const char* pFileName);
	void CreateBack(const char* pFileName, D3DXCOLOR col);

	// 設定
	void Set(float fBase, D3DXCOLOR col);
	void SetBase(float base) { m_fBase = base; }
	void SetRate(float rate) { m_fRate = rate; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	// 静的メンバ関数
	static CGaugeBillboard* Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size = D3DXVECTOR2(50.0f, 50.0f));

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR2 m_size;					// サイズ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXCOLOR m_col;					// 色
	float m_fBase;						// 基準
	float m_fMaxBase;					// 最大基準
	float m_fRate;						// レート
	int m_nIdxTex;						// テクスチャのインデックス番号

};
#endif
