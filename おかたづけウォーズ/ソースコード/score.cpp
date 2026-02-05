//===============================================
//
// スコア[score.h]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "score.h"
#include "renderer.h"
#include "manager.h"

//===============================================
// コンストラクタ
//===============================================
CScore::CScore(int nPriority) :CObject(nPriority)
{
	// 値をクリア
	m_nScore = 0;
	m_nMaxScore = 0;
	m_nScoreData = 0;
}
//===============================================
// デストラクタ
//===============================================
CScore::~CScore()
{
}
//===============================================
// 生成処理
//===============================================
CScore* CScore::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, int MaxScore, int ScoreData)
{
	CScore* pScore = new CScore;
	// 初期化
	pScore->Init(pos, fWidth, fHeight, MaxScore, ScoreData);

	return pScore;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fWidth, float fHeight, int MaxScore, int ScoreData)
{
	// スコアの初期化
	m_nScore = 0;
	m_fWidth = fWidth;
	m_nMaxScore = MaxScore;
	m_nScoreData = ScoreData;

	// 数字の生成
	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		m_pNumber[nCnt] = CNumber::Create("data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (nCnt * fWidth * 1.5f), pos.y, 0.0f), fWidth, fHeight);
	}

	// オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CScore::Uninit(void)
{
	// 数字の終了
	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		if (m_pNumber != nullptr)
		{
			// 終了処理/破棄
			m_pNumber[nCnt]->Uninit();
			delete m_pNumber[nCnt];
			m_pNumber[nCnt] = nullptr;
		}
	}
	// オブジェクトの破棄
	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CScore::Update(void)
{
	//数字切り替え
	ChangeTex();
}
//===============================================
// 描画処理
//===============================================
void CScore::Draw(void)
{
	// 数字の描画
	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		// NULLチェック
		if (m_pNumber[nCnt] != nullptr)
		{
			// 描画
			m_pNumber[nCnt]->Draw();
		}
	}
}
//===============================================
// スコア加算
//===============================================
void CScore::AddScore(int nAdd)
{
	// スコアを加算
	m_nScore += nAdd;
}
//===============================================
// 数字のテクスチャ変更
//===============================================
void CScore::ChangeTex(void)
{
	int aPosTexU;
	if (m_nScore <= 0)
	{
		m_nScore = 0;
	}

	int nData = m_nScoreData * 10;
	int nData2 = m_nScoreData;
	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		// NULLチェック
		if (m_pNumber[nCnt] != nullptr)
		{
			aPosTexU = m_nScore % nData / nData2;
			nData /= 10;
			nData2 /= 10;
			// NULLチェック
			if (m_pNumber[nCnt] != nullptr)
			{
				// テクスチャ変更
				m_pNumber[nCnt]->SetTex((aPosTexU * 0.1f), (aPosTexU * 0.1f) + 0.1f, 0.0f, 1.0f);
			}
		}
	}
}
//===============================================
// 位置設定
//===============================================
void CScore::SetPos(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		m_pNumber[nCnt]->SetPos(D3DXVECTOR3(pos.x + (nCnt * m_fWidth * 1.5f), pos.y, 0.0f));
	}
}
//===============================================
// 点滅
//===============================================
void CScore::Blink(void)
{
	D3DXCOLOR col = m_pNumber[0]->GetColor();

	col.a -= 0.05f;
	if (col.a <= 0.0f)
	{
		col.a = 1.0f;
	}

	for (int nCnt = 0; nCnt < m_nMaxScore; nCnt++)
	{
		// NULLチェック
		if (m_pNumber[nCnt] != nullptr)
		{
			m_pNumber[nCnt]->SetColor(col);
		}
	}
}