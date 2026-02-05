//==============================================
//
// ランキングマネージャー[rank_manager.cpp]
// Author : Kaiti Aoto
//
//==============================================

// 二重インクルード防止
#ifndef RANK_MANAGER_H_
#define RANK_MANAGER_H_

// インクルード
#include "main.h"
#include "score_manager.h"

// マクロ定義
#define MAX_RANK (5)
#define RANK_Y (4)

// ランキングマネージャークラス
class CRankMana
{
public:

	// メンバ関数
	CRankMana();
	~CRankMana();
	HRESULT Init(D3DXVECTOR3 pos, const char* pTxtName, D3DXCOLOR col, int MaxScore);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reset(void);

	// 設定
	void Set(int nScore);
	void SetRankIn(int nScore);

	// 静的メンバ関数
	static CRankMana* Create(D3DXVECTOR3 pos, const char* pTxtName, D3DXCOLOR col, int MaxScore);

private:
	void Move(void);
	void Blink(void);

	// メンバ変数
	CScoreMana* m_pScore[MAX_RANK];	// スコアマネージャーへのポインタ
	D3DXVECTOR3 m_posOffset;		// 位置(オフセット)
	D3DXVECTOR3 m_pos;				// 位置
	float m_fSize;					// サイズ
	float m_AnimCnt[MAX_RANK];		// アニメカウント
	int m_nRankIn;					// ランクインできた順位
	int m_BlinkTime;				// 点滅時間
	int m_nScore[MAX_RANK];			// スコア
	char m_pTxtName[64];			// テキスト名
};

#endif