//===============================================
//
// 収集エリアマネージャー[gatherAreaManager.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GATHERAREAMANAGER_H_
#define _GATHERAREAMANAGER_H_

// インクルード
#include "main.h"
#include "object.h"
#include "gatherArea.h"

// 収集エリアマネージャークラス
class CGatherAreaManager :public CObject
{
public:
	// ウェーブの種類
	typedef enum
	{
		WAVE_0 = 0,
		WAVE_1,
		WAVE_2,
		WAVE_3,
		WAVE_MAX
	}WAVE;

	// メンバ関数
	CGatherAreaManager(int nPriority = static_cast<int>(Priority::Character));
	~CGatherAreaManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetWave(void);

	// 静的メンバ関数
	static CGatherAreaManager* Create(void);

private:
	void LoadWave(void);
	void ResetArea(void);

	// 定数定義
	static constexpr int GATHERTIME = 1200;	// 回収時間

	// メンバ変数
	WAVE m_wave;						// ウェーブの種類
	int m_nNumArea;						// エリア数
	std::vector<CGatherArea*> m_pArea;	// エリアを動的配列

	// 静的メンバ変数
	static int m_nCntTime;				// 回収時間計測用

};

#endif