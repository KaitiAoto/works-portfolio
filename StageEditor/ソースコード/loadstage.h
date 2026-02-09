//==============================
//
// テクスチャ[texture.h]
// Author:kaiti
//
//==============================
#ifndef LOADSTAGE_H_
#define LOADSTAGE_H_

#include "main.h"
#include "stage.h"

#define NUM_STAGE_MODEL (32)
#define MAX_STAGE_MODEL (512)
//オブジェクトクラス
class CLoadStage
{
public:

	//メンバ関数
	CLoadStage();
	~CLoadStage();
	void Load(const char* pFilename);
	void Unload(void);

private:
	void Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type, int nIndx);

	//
	CStage* m_pStage[MAX_STAGE_MODEL];
	int m_nNumModel;

	int m_nCntStage;
};

#endif