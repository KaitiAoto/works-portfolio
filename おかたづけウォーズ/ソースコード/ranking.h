//===============================================
//
// ランキング[ranking.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _RANKING_H_
#define _RANKING_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "rank_manager.h"

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
	static CRankMana* GetRankMana(void) { return m_pRank; }

private:

	static constexpr int RANK_STAY = 90;	// 待機時間

	// メンバ変数
	int m_nCntStay;					// 待機時間計測用

	// 静的メンバ変数
	static CLoadStage* m_pStage;	// 読み込むステージへのポインタ
	static CRankMana* m_pRank;		// トータルランキングマネージャーへのポインタ

};

#endif