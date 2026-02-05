//===================
//
// 矢印[arrow.h]
// Author : Kaiti Aoto
//
//===================

// 二重インクルード防止
#ifndef _ARROW_H_
#define _ARROW_H_

// インクルード
#include "main.h"
#include "object3D.h"

// 矢印クラス
class CArrow :public CObject3D
{
public:

	//メンバ関数
	CArrow(int nPriority = 9);
	~CArrow();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理

	//設定処理
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CArrow* Create(D3DXVECTOR3 pos);

private:
	//メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_TargetPos;// 目標の位置
	bool m_bUse;			// 使用しているか
};

#endif