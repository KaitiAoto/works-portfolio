//========================
//
// スコア[score.h]
// Author : Kaiti Aoto
//
//========================

// 二重インクルード防止
#ifndef _SCORE_H_
#define _SCORE_H_

// インクルード
#include "main.h"
#include "number.h"

// マクロ定義
#define SCORE_SIZE (20)

//オブジェクト2Dクラス
class CScore:public CObject
{
public:

	//メンバ関数
	CScore(int nPriority = 8);
	~CScore();
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static int GetPriority(void) { return m_nPriority; }
	CNumber* GetNumber(void) { return m_pNumber; }
	void SetColor(D3DXCOLOR col);
private:
	//メンバ変数
	static int m_nPriority;
	int m_nIdxTex;
	CNumber* m_pNumber;
};

#endif