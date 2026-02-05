//===============================================
//
//　ライト処理[light.h]
//　Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _LIGHT_H_
#define _LIGHT_H_

// インクルード
#include "main.h"

// クラス
class CLight
{
public:

	// メンバ関数
	CLight();
	// 仮想関数
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// 設定
	void SetColor(D3DXCOLOR col);
private:

	static constexpr int MAX_LIGHT = 3;	// 最大ライト数

	// メンバ変数
	D3DLIGHT9 m_light[MAX_LIGHT]; // ライト情報

};

#endif