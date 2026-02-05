//===============================================
//
//  モデル矢印処理[arrow.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _ARROW_H_
#define _ARROW_H_

// インクルード
#include "objectX.h"

// ビルボードゲージクラス
class CArrow :public CObjectX
{
public:

	// メンバ関数
	CArrow(int nPriority = static_cast<int>(Priority::StageUI));
	~CArrow();
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定

	// 静的メンバ関数
	static CArrow* Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 10.0f, 10.0f));

private:
	// メンバ変数
	D3DXVECTOR3 m_BasePos;		// 基本位置
	D3DXVECTOR3 m_size;			// サイズ
	D3DXCOLOR m_col;			// 色
	bool m_bUp;					// 上昇するか
};
#endif
