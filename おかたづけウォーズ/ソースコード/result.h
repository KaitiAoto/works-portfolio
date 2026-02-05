//===============================================
//
// リザルト処理[result.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _RESULT_H_
#define _RESULT_H_

//インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "score.h"
#include "timer.h"
#include "gauge.h"

// 前方宣言
class CScene;
class CLoadStage;

// リザルトクラス
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

	// 取得
	CScore* GetScore(void) { return m_pScore; }

private:

	static constexpr int RESULT_STAY = 60;		// 待機時間
	static constexpr float SCORE_SIZE = 20.0f;	// スコアの大きさ

	// メンバ変数
	int m_nCntStay;					// 待機時間計測用
	CScore* m_pScore;				// スコアへのポインタ
	CScore* m_pMissCnt;				// ミス数へのポインタ
	CScore* m_pAttackCnt;			// 攻撃数へのポインタ
	CGauge* m_pGauge;				// ＨＰゲージ

};

#endif