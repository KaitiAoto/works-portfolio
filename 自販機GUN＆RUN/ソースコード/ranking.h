//================================
//
// ランキング[ranking.h]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _RANKING_H_
#define _RANKING_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "rank_manager.h"

// マクロ定義
#define RANK_STAY (90)
#define RANKSCORE_SIZE (15)

// 前方宣言
class CScene;
class CLoadStage;

// ランキングクラス
class CRanking:public CScene
{
public:
	// メンバ関数
	CRanking();
	~CRanking();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// 取得
	static CRankMana* GetBreakRankMana(void) { return m_pBreakRank; }
	static CRankMana* GetTotalRankMana(void) { return m_pTotalRank; }
	// 設定
	static void SetNowScore(int nBreakScore, int nTotalScore);
private:
	// メンバ変数
	int m_nCntStay;					// 待機時間

	// 静的メンバ変数
	static CLoadStage* m_pStage;	// 読み込むステージへのポインタ
	static CRankMana* m_pBreakRank;	// 破壊ランキングマネージャーへのポインタ
	static CRankMana* m_pTotalRank;	// トータルランキングマネージャーへのポインタ

};

#endif