//===============================================
//
// スコア[score.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _SCORE_H_
#define _SCORE_H_

// インクルード
#include "main.h"
#include "number.h"
#include "file.h"

// スコアクラス
class CScore:public CObject
{
public:

	// メンバ関数
	CScore(int nPriority = static_cast<int>(Priority::UI_Front));
	~CScore();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int MaxScore = MAX_SCORE, int ScoreData = SCOREDATA);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddScore(int nAdd);
	void Save(const char* pFilename) { saveData.push_back(m_nScore); CFile::IntSave(pFilename, saveData); }
	void Blink(void);
	// 設定
	void SetScore(int nScore) { m_nScore = nScore; }
	void SetPos(D3DXVECTOR3 pos);
	// 取得
	int GetScore(void) { return m_nScore; }

	// 静的メンバ関数
	static CScore* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int MaxScore = MAX_SCORE,int ScoreData = SCOREDATA);
	static std::vector<int> Load(const char* pFilename) { return CFile::IntLoad(pFilename); }

private:
	void ChangeTex(void);

	static constexpr int MAX_SCORE = 8;			// 最大桁数
	static constexpr int SCOREDATA = 10000000;	// テクスチャ変更用

	// メンバ変数
	int m_nScore;					// 現在のスコア
	float m_fWidth;					// 横幅
	CNumber* m_pNumber[MAX_SCORE];	// 数字へのポインタ
	int m_nMaxScore;
	int m_nScoreData;
	std::vector<int> saveData;
};

#endif