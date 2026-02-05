//===============================================
//
//  モデルゲージ処理[gaugeModel.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GAUGEMODEL_H_
#define _GAUGEMODEL_H_

// インクルード
#include"main.h"
#include "objectX.h"

// ビルボードゲージクラス
class CGaugeModel :public CObjectX
{
public:

	// メンバ関数
	CGaugeModel(int nPriority = static_cast<int>(Priority::StageUI));
	~CGaugeModel();
	HRESULT Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 100.0f, 100.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateFrame(const char* pFileName, D3DXCOLOR col);
	void CreateBack(const char* pFileName, D3DXCOLOR col);

	// 設定
	void Set(float fBase, D3DXCOLOR col);
	void SetBase(float base) { m_fBase = base; }
	void SetRate(float rate) { m_fRate = rate; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void SetShakeInfo(bool bShake, int nShakeTime, D3DXVECTOR3 MaxShake);
	// 静的メンバ関数
	static CGaugeModel* Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 100.0f, 100.0f));

private:
	// メンバ変数
	D3DXVECTOR3 m_size;					// サイズ
	D3DXCOLOR m_col;					// 色
	CObjectX* m_pFrame;					// フレームへのポインタ
	CObjectX* m_pBack;					// 背景へのポインタ
	float m_fBase;						// 基準
	float m_fMaxBase;					// 最大基準
	float m_fRate;						// レート

};
#endif
