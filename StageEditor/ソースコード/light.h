//================================
//
//　ライト処理[light.h]
//　Author:kaiti
//
//================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

#define MAX_LIGHT (3)

//クラス
class CLight
{
public:

	//メンバ関数
	CLight();
	//仮想関数
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	D3DLIGHT9 m_light[MAX_LIGHT]; //ライト情報

};

#endif