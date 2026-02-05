//========================================
//
// スコアマネージャー[score_manager.h]
// Author : Kaiti Aoto
//
//========================================
#include "score_manager.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CScoreMana::CScoreMana()
{
	m_MaxScore = 0;
	m_ScoreData = 0;
}
//================
// デストラクタ
//================
CScoreMana::~CScoreMana()
{
}
//===========
// 生成処理
//===========
CScoreMana* CScoreMana::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int MaxScore)
{
	CScoreMana* pScoreMana = new CScoreMana;

	for (int nCnt = 0; nCnt < MaxScore; nCnt++)
	{
		pScoreMana->m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(pos.x + (nCnt * fWidth * 2.5f), pos.y, 0.0f), fWidth, fHeight);
		pScoreMana->m_pScore[nCnt]->SetColor(col);
		pScoreMana->m_col = col;
	}
	pScoreMana->m_fWidth = fWidth;

	pScoreMana->Init(MaxScore);

	return pScoreMana;
}
CScoreMana* CScoreMana::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int MaxScore, TYPE type)
{
	CScoreMana* pScoreMana = new CScoreMana;

	for (int nCnt = 0; nCnt < MaxScore; nCnt++)
	{
		pScoreMana->m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(pos.x + (nCnt * fWidth * 2.5f), pos.y, 0.0f), fWidth, fHeight);
		pScoreMana->m_pScore[nCnt]->SetColor(col);
		pScoreMana->m_col = col;
	}
	pScoreMana->m_fWidth = fWidth;

	pScoreMana->Init(MaxScore);
	pScoreMana->m_type = type;

	return pScoreMana;
}
//===============
// 初期化処理
//===============
HRESULT CScoreMana::Init(int MaxScore)
{
	//初期化
	m_nScore = 0;
	m_nCntUp = 0;
	
	m_MaxScore = MaxScore;
	m_type = TYPE_CNTUP;

	m_ScoreData = 1;
	for (int nCnt = 0; nCnt < m_MaxScore - 1; nCnt++)
	{
		m_ScoreData *= 10;
	}

	return S_OK;
}
//============
// 終了処理
//============
void CScoreMana::Uninit(void)
{
	m_nScore = 0;
}
//============
// 更新処理
//============
void CScoreMana::Update(void)
{
	if (m_type == TYPE_CNTUP)
	{
		if (m_nCntUp < m_nScore)
		{
			//目標のスコアと表示のスコアの差分を計算
			int nDiff = m_nScore - m_nCntUp;

			//差分の10分の１の値を足す
			int nAdd = max(1, nDiff / 10);//足す値を計算
			m_nCntUp += nAdd;//加算

			if (m_nCntUp >= m_nScore)
			{//目標の値を超えないようにする
				m_nCntUp = m_nScore;
			}
		}
	}
	else
	{
		m_nCntUp = m_nScore;
	}

	//数字切り替え
	ChangeTex();
}
//============
// 描画処理
//============
void CScoreMana::Draw(void)
{
}
//==============
// スコア加算
//==============
void CScoreMana::AddScore(int nAdd)
{
	if (CGame::GetMode() != CGame::MODE_TUTORIAL)
	{
		m_nScore += nAdd;
	}
}
//================
// 位置設定
//================
void CScoreMana::SetPos(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < m_MaxScore; nCnt++)
	{
		m_pScore[nCnt]->GetNumber()->SetPos({ pos.x + (nCnt * m_fWidth * 2.5f), pos.y, pos.z });
	}
}
//============
// 色設定
//============
void CScoreMana::SetAlpha(float fAlpha)
{
	m_col.a = fAlpha;

	for (int nCnt = 0; nCnt < m_MaxScore; nCnt++)
	{
		m_pScore[nCnt]->GetNumber()->SetColor((m_col));
	}
}
//===================
// 数字切り替え
//===================
void CScoreMana::ChangeTex(void)
{
	int aPosTexU;
	if (m_nCntUp <= 0)
	{
		m_nCntUp = 0;
	}

	int nData = m_ScoreData * 10;
	int nData2 = m_ScoreData;
	for (int nCnt = 0; nCnt < m_MaxScore; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{
			CNumber* pNumber = m_pScore[nCnt]->GetNumber();

			aPosTexU = m_nCntUp % nData / nData2;
			nData /= 10;
			nData2 /= 10;
			if (pNumber != nullptr)
			{
				pNumber->SetTex((aPosTexU * 0.1f), (aPosTexU * 0.1f) + 0.1f, 0.0f, 1.0f);
			}
		}
	}
}
