//==============================
//
// テクスチャ[texture.h]
// Author:kaiti
//
//==============================
#ifndef SAVESTAGE_H_
#define SAVESTAGE_H_

#include "main.h"
#include "stage.h"
#include "loadstage.h"
#include "object3D.h"

//オブジェクトクラス
class CSaveStage
{
public:

	//メンバ関数
	CSaveStage();
	~CSaveStage();
	void Save(const char* pFilename);
	void Unload(void);
	void AddModel(CStage* pStage) {if (m_nCntStage < MAX_STAGE_MODEL){m_pStage[m_nCntStage] = pStage;m_nCntStage++;}}
	void AddField(CObject3D* pField) { if (m_nCntField < MAX_STAGE_MODEL) { m_pField[m_nCntField] = pField; m_nCntField++; } }
	void AddWall(CObject3D* pField) { if (m_nCntWall < MAX_STAGE_MODEL) { m_pWall[m_nCntWall] = pField; m_nCntWall++; } }

private:
	void Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type, int nIndx);

	//
	CStage* m_pStage[MAX_STAGE_MODEL];
	CObject3D* m_pField[MAX_STAGE_MODEL];
	CObject3D* m_pWall[MAX_STAGE_MODEL];

	int m_nNumModel;

	int m_nCntStage;
	int m_nCntField;
	int m_nCntWall;

};

#endif