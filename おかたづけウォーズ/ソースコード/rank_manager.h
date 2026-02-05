//===============================================
//
// ランキングマネージャー[rank_manager.cpp]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef RANK_MANAGER_H_
#define RANK_MANAGER_H_

// インクルード
#include "main.h"
#include "score.h"

// ランキングマネージャークラス
class CRankMana
{
public:

	// メンバ関数
	CRankMana();
	~CRankMana();
	HRESULT Init(D3DXVECTOR3 pos, const char* pTxtName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reset(void);

	// 設定
	void Set(int nScore);
	void SetRankIn(int nScore);

	// 静的メンバ関数
	static CRankMana* Create(D3DXVECTOR3 pos, const char* pTxtName);

private:
	void Move(void);

	// 定数定義
	static constexpr int MAX_RANK = 5;				// 最大順位
	static constexpr int RANK_Y = 3;				// ランク位置調整用の値
	static constexpr float RANKSCORE_SIZE = 37.5;	// ランクスコアのサイズ

	// メンバ変数
	D3DXVECTOR3 m_posOffset;		// 位置(オフセット)
	D3DXVECTOR3 m_pos;				// 位置
	CScore* m_pScore[MAX_RANK];		// スコアへのポインタ
	float m_fSize;					// サイズ
	float m_AnimCnt[MAX_RANK];		// アニメカウント
	int m_nRankIn;					// ランクインできた順位
	int m_BlinkTime;				// 点滅時間
	int m_nScore[MAX_RANK];			// スコア
	char m_pTxtName[64];			// テキスト名
};

#endif