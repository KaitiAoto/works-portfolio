//===============================================
//
// ランキングマネージャー[rank_manager.h]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "rank_manager.h"
#include "renderer.h"
#include "manager.h"
#include "easing.h"
#include "debugproc.h"

//===============================================
// コンストラクタ
//===============================================
CRankMana::CRankMana()
{
	// 値をクリア
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
//===============================================
// デストラクタ
//===============================================
CRankMana::~CRankMana()
{
}
//===============================================
// 生成処理
//===============================================
CRankMana* CRankMana::Create(D3DXVECTOR3 pos, const char* pTxtName)
{
	// インスタンス生成
	CRankMana* pRankMana = new CRankMana;

	// 初期化
	if (FAILED(pRankMana->Init(pos, pTxtName)))
	{
		delete pRankMana;
		return nullptr;
	}

	return pRankMana;
}
//===============================================
// リセット
//===============================================
void CRankMana::Reset(void)
{
	// ランキングファイルを読み込む
	std::vector<int> data = CFile::IntLoad(m_pTxtName);

	if (data.size() >= MAX_RANK)
	{
		// 読み込んだデータを格納
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_nScore[nCnt] = data[nCnt];
		}
	}
	else
	{
		// 全て初期化
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_nScore[nCnt] = 0;
		}
	}
}
//===============================================
// 初期化処理
//===============================================
HRESULT CRankMana::Init(D3DXVECTOR3 pos, const char* pTxtName)
{
	// オフセットを設定
	m_posOffset = pos;

	// テキスト名を設定
	strcpy(m_pTxtName, pTxtName);

	// ランキングをリセット
	Reset();

	// 順位テクスチャ
	const char* apFileName[MAX_RANK] =
	{
		"data\\TEXTURE\\rank01.png",
		"data\\TEXTURE\\rank02.png",
		"data\\TEXTURE\\rank03.png",
		"data\\TEXTURE\\rank04.png",
		"data\\TEXTURE\\rank05.png",
	};

	// サイズを設定
	m_fSize = RANKSCORE_SIZE;

	// ランキングスコア初期化
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		// スコア生成＆値を設定
		m_pScore[nCnt] = CScore::Create(D3DXVECTOR3(0.0f, pos.y + (nCnt * m_fSize * RANK_Y), 0.0f), m_fSize, m_fSize);
		m_pScore[nCnt]->SetScore(m_nScore[nCnt]);

		// 順位アイコン生成
		CObject2D::Create(apFileName[nCnt], D3DXVECTOR3(pos.x - (m_fSize * 3.0f), pos.y + (nCnt * m_fSize * RANK_Y), 0.0f), VECTOR3_NULL, D3DXVECTOR2(m_fSize * 2.0f, m_fSize * 2.0f), 7);
	}

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CRankMana::Uninit(void)
{
}
//===============================================
// 更新処理
//===============================================
void CRankMana::Update(void)
{
	// 移動
	Move();

	// 各順位ごとに更新
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		// NULLチェック
		if (m_pScore[nCnt] != nullptr)
		{
			// 更新
			m_pScore[nCnt]->Update();

			// ランクインしたら
			if (nCnt == m_nRankIn)
			{
				// 点滅
				m_pScore[nCnt]->Blink();
			}
		}
	}
}
//===============================================
// 移動
//===============================================
void CRankMana::Move(void)
{
	const float MoveDuration = 120.0f;  // 1つのスコアが滑り込むフレーム数
	const float Stagger = 10.0f;        // 順番にずらすフレーム差
	const float startX = -500.0f;		// 開始時のX座標

	// 各順位ごとに処理
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		// カウンターが一定より下なら
		if (m_AnimCnt[nCnt] < MoveDuration + nCnt * Stagger)
		{
			// カウンターを進める
			m_AnimCnt[nCnt] += 1.0f;
		}

		// 遅延
		float t = (m_AnimCnt[nCnt] - nCnt * Stagger) / MoveDuration;
		// ０より下なら
		if (t < 0.0f)
		{
			// ０にする
			t = 0.0f;
		}

		float posX;
		// １以上なら
		if (t >= 1.0f)
		{
			// 位置を固定
			posX = m_posOffset.x;
		}
		else
		{
			// 弾性イージング
			float ease = CEasing::OutElastic(t);
			posX = startX + (m_posOffset.x - startX) * ease;
		}

		// デバッグ表示
		CDebugProc* pDegub = CManager::GetDebug();
		pDegub->Print("位置：%.1f", posX);

		// 位置更新
		m_pScore[nCnt]->SetPos(D3DXVECTOR3(posX, m_posOffset.y + (nCnt * m_fSize * RANK_Y), 0.0f));
	}
}
//===============================================
// 描画処理
//===============================================
void CRankMana::Draw(void)
{
}
//===============================================
// 設定
//===============================================
void CRankMana::Set(int nScore)
{
	int aTempScore[MAX_RANK + 1];	// 今回のスコアを含めたソート用スコア

	// ランキングファイルを読み込む
	std::vector<int> data = CFile::IntLoad(m_pTxtName);

	if (data.size() >= MAX_RANK)
	{
		// 読み込んだデータを格納
		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			m_nScore[nCnt] = data[nCnt];		// ランキングスコアに保存
			aTempScore[nCnt] = m_nScore[nCnt];	// ソート用スコアに代入
		}
		// 今回のスコアを保存
		aTempScore[MAX_RANK] = nScore;
	}

	// ソート
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		for (int nCnt1 = 0; nCnt1 < MAX_RANK - nCnt; nCnt1++)
		{
			if (aTempScore[nCnt1] < aTempScore[nCnt1 + 1])
			{
				int tmp = aTempScore[nCnt1];
				aTempScore[nCnt1] = aTempScore[nCnt1 + 1];
				aTempScore[nCnt1 + 1] = tmp;
			}
		}
	}
	// 上位を保存
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		m_nScore[nCnt] = aTempScore[nCnt];
		if (m_pScore[nCnt] != nullptr)
		{
			m_pScore[nCnt]->SetScore(m_nScore[nCnt]);
		}
	}

	// ランクインしたか調べる
	SetRankIn(nScore);

	// ランキング書き込み用
	std::vector<int> saveData;

	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{// 最大ランク分書き込む
		saveData.push_back(m_nScore[nCnt]);
	}
	// ランキングファイルに書き込み
	CFile::IntSave(m_pTxtName, saveData);
}
//===============================================
// ランクインしているか
//===============================================
void CRankMana::SetRankIn(int nScore)
{
	// ランキングの中に今回のスコアがあれば
	for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if (m_nScore[nCnt] == nScore)
		{
			m_nRankIn = nCnt;	// ランクインした順位を保存
			break;
		}
	}
}