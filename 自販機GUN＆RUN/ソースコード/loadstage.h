//==============================
//
// ステージ読み込み[loadstage.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef LOADSTAGE_H_
#define LOADSTAGE_H_

// インクルード
#include "main.h"
#include "stage.h"

// マクロ定義
#define NUM_STAGE_MODEL (64)
#define MAX_STAGE_MODEL (512)
#define NUM_STAGE_TEXTURE (16)

// ステージ読み込みクラス
class CLoadStage
{
public:
	// メンバ関数
	CLoadStage();
	~CLoadStage();
	void Load(const char* pFilename);
	void Unload(void);

private:
	void Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type);

	// メンバ変数
	int m_nNumModel;	// 最大モデル数
	int m_nCntStage;	// ステージモデルカウント
};

#endif