//========================================
//
// スコアマネージャー[score_manager.h]
// Author : Kaiti Aoto
//
//========================================
#include "rank_manager.h"
#include "renderer.h"
#include "manager.h"
#include "easing.h"
#include "debugproc.h"
//==================
// コンストラクタ
//==================
CRankMana::CRankMana()
{
	m_nRankIn = -1;
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_AnimCnt[nCnt] = 0.0f;
	}
	for (int nCnt = 0; nCnt < 64; nCnt++)
	{
		m_pTxtName[nCnt] = {};
	}
}
//================
// デストラクタ
//================
CRankMana::~CRankMana()
{
}
//===========
// 生成処理
//===========
CRankMana* CRankMana::Create(D3DXVECTOR3 pos, const char* pTxtName, D3DXCOLOR col, int MaxScore)
{
	CRankMana* pScoreMana = new CRankMana;

	pScoreMana->Init(pos, pTxtName, col, MaxScore);

	return pScoreMana;
}
//
//
//
void CRankMana::Reset(void)
{
	FILE* pFile;
	// ランキングファイルを読み込んで m_nScore に格納
	pFile = fopen(m_pTxtName, "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &m_nScore[nCnt]);
		}
		fclose(pFile);
	}
	else
	{
		// ファイルが存在しない場合は全て 0 で初期化
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_nScore[nCnt] = 0;
		}
	}
}
//===============
// 初期化処理
//===============
HRESULT CRankMana::Init(D3DXVECTOR3 pos, const char* pTxtName, D3DXCOLOR col, int MaxScore)
{
	strcpy(m_pTxtName, pTxtName);

	Reset();

	//順位テクスチャ
	const char* apFileName[MAX_RANK] =
	{
		"data\\TEXTURE\\rank01.png",
		"data\\TEXTURE\\rank02.png",
		"data\\TEXTURE\\rank03.png",
		"data\\TEXTURE\\rank04.png",
		"data\\TEXTURE\\rank05.png",
	};

	float fSize = RANKSCORE_SIZE * 1.5f;
	m_posOffset = pos;
	m_fSize = fSize;

	//初期化
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_pScore[nCnt] = CScoreMana::Create(D3DXVECTOR3(0.0f, pos.y + (nCnt * fSize * RANK_Y), 0.0f), fSize, fSize, col, MaxScore);
		m_pScore[nCnt]->Set(m_nScore[nCnt]);

		CObject2D::Create(apFileName[nCnt], D3DXVECTOR3(pos.x - (fSize * 4.0f), pos.y + (nCnt * fSize * RANK_Y), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fSize * 2.0f, fSize * 2.0f, 7);
	}

	return S_OK;
}
//============
// 終了処理
//============
void CRankMana::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{
			m_pScore[nCnt]->Uninit();
			delete m_pScore[nCnt];
			m_pScore[nCnt] = nullptr;
		}
	}
}
//============
// 更新処理
//============
void CRankMana::Update(void)
{
	Move();

	if (m_nRankIn != -1)
	{
		Blink();
	}
	else
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_pScore[nCnt]->SetAlpha(1.0f);
		}
	}
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_pScore[nCnt]->Update();
	}
}
//===========
// 移動
//===========
void CRankMana::Move(void)
{
	const float MoveDuration = 120.0f;  //1つのスコアが滑り込むフレーム数
	const float Stagger = 10.0f;        //順番にずらすフレーム差
	const float startX = -500.0f;		//開始時のX座標

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_AnimCnt[nCnt] < MoveDuration + nCnt * Stagger)
		{
			m_AnimCnt[nCnt] += 1.0f;
		}

		//遅延
		float t = (m_AnimCnt[nCnt] - nCnt * Stagger) / MoveDuration;
		if (t < 0.0f) t = 0.0f;

		float posX;
		if (t >= 1.0f)
		{
			posX = m_posOffset.x;
		}
		else
		{
			// 弾性イージング
			float ease = CEasing::OutElastic(t);
			posX = startX + (m_posOffset.x - startX) * ease;
		}

		CDebugProc* pDegub = CManager::GetDebug();
		pDegub->Print("位置：%.1f", posX);

		//位置更新
		m_pScore[nCnt]->SetPos(D3DXVECTOR3(posX, m_posOffset.y + (nCnt * m_fSize * RANK_Y), 0.0f));
	}
}
//============
// 点滅
//============
void CRankMana::Blink(void)
{
	const int nBlinkFrame = 20;//点滅フレーム数
	const int nBlinkTime = 60;
	const D3DXCOLOR BaseCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR BlinkCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	m_BlinkTime++;

	if ((m_BlinkTime / nBlinkFrame) % 2 == 0)
	{//ダメージ色にする
		if (m_pScore[m_nRankIn] != NULL)
		{
			m_pScore[m_nRankIn]->SetAlpha(0.0f);
		}
	}
	else
	{//通常色に戻す
		if (m_pScore[m_nRankIn] != NULL)
		{
			m_pScore[m_nRankIn]->SetAlpha(1.0f);
		}
	}
	if (m_BlinkTime >= nBlinkTime)
	{
		m_BlinkTime = 0;
	}
}

//============
// 描画処理
//============
void CRankMana::Draw(void)
{
}
//
//
//
void CRankMana::Set(int nScore)
{
	FILE* pFile;
	// ファイルから既存スコアを読み込む
	pFile = fopen(m_pTxtName, "r");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d\n", &m_nScore[nCnt]);
		}
		fclose(pFile);
	}

	int tempScore[MAX_RANK + 1];
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		tempScore[nCnt] = m_nScore[nCnt];
	}
	tempScore[MAX_RANK] = nScore;

	//ソート
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		for (int j = 0; j < MAX_RANK - nCnt; j++)
		{
			if (tempScore[j] < tempScore[j + 1])
			{
				int tmp = tempScore[j];
				tempScore[j] = tempScore[j + 1];
				tempScore[j + 1] = tmp;
			}
		}
	}
	//上位を保存
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_nScore[nCnt] = tempScore[nCnt];
		if (m_pScore[nCnt] != nullptr)
		{
			m_pScore[nCnt]->Set(m_nScore[nCnt]);
		}
	}

	//ファイルに書き込む
	pFile = fopen(m_pTxtName, "w");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{//最大ランク分書き込む
			fprintf(pFile, "%d\n", m_nScore[nCnt]);
		}
		//閉じる
		fclose(pFile);
	}
	else
	{//エラーチェック
	}
}

void CRankMana::SetRankIn(int nScore)
{
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_nScore[nCnt] == nScore)
		{
			m_nRankIn = nCnt;
			break;
		}
	}
}

