//===============================================
//
// ステージ読み込み[loadstage.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef LOADSTAGE_H_
#define LOADSTAGE_H_

// インクルード
#include "main.h"
#include "stage.h"

// ステージ読み込みクラス
class CLoadStage
{
public:
	// メンバ関数
	CLoadStage();
	~CLoadStage();
	void Load(const char* pFilename);

private:
	void Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type, bool bShadow);

	static constexpr int NUM_STAGE_MODEL = 64;		// 最大ステージモデル種類数
	static constexpr int MAX_STAGE_MODEL = 512;		// 最大ステージモデル数
	static constexpr int NUM_STAGE_TEXTURE = 16;	// 最大テクスチャ種類数

	// メンバ変数
	int m_nNumModel;	// 最大モデル数
	int m_nCntStage;	// ステージモデルカウント
};

#endif