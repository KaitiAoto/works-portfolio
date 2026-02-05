//================================
//
// リザルト処理[result.h]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _RESULT_H_
#define _RESULT_H_

//インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "score_manager.h"

// マクロ定義
#define RESULT_STAY (90)

// 前方宣言
class CScene;
class CLoadStage;

// マネージャークラス
class CResult:public CScene
{
public:
	// メンバ関数
	CResult();
	~CResult();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// 取得
	static CScoreMana* GetBreakCnt(void) { return m_pBreakCnt; }
	static CScoreMana* GetTotalScore(void) { return m_pTotalScore; }

private:

	// メンバ変数
	int m_nCntStay;						// 待機時間

	// 静的メンバ変数
	static CLoadStage* m_pStage;		// 読み込むステージへのポインタ
	static CScoreMana* m_pBreakCnt;		// 破壊スコアマネージャーへのポインタ
	static CScoreMana* m_pTotalScore;	// トータルスコアマネージャーへのポインタ
};

#endif