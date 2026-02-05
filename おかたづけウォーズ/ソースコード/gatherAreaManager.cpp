//===============================================
//
// 収集エリアマネージャー[gatherAreaManager.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "gatherAreaManager.h"
#include "interact.h"
#include "game.h"
#include "manager.h"

// 静的メンバ変数
int CGatherAreaManager::m_nCntTime = 0;

//===============================================
// コンストラクタ
//===============================================
CGatherAreaManager::CGatherAreaManager(int nPriority):CObject(nPriority)
{
	// 値をクリア
	m_wave = WAVE_0;
	m_nNumArea = 0;
	m_nCntTime = 0;
}	
//===============================================
// デストラクタ
//===============================================
CGatherAreaManager::~CGatherAreaManager()
{
}
//===============================================
// 生成処理
//===============================================
CGatherAreaManager* CGatherAreaManager::Create(void)
{
	// インスタンス生成
	CGatherAreaManager* pArea = new CGatherAreaManager;

	// 初期化
	if (FAILED(pArea->Init()))
	{
		delete pArea;
		return nullptr;
	}

	return pArea;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CGatherAreaManager::Init(void)
{
	// ウェーブセット
	SetWave();

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CGatherAreaManager::Uninit(void)
{
	// エリアをリセット
	ResetArea();

	// オブジェクト破棄
	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CGatherAreaManager::Update(void)
{
	// カウンターを回す
	m_nCntTime++;

	// 全エリア分
	for (int nCnt = 0; nCnt < m_nNumArea; nCnt++)
	{
		// NULLチェック
		if (m_pArea[nCnt] != nullptr)
		{
			// 一定時間経過したら
			if (m_nCntTime >= GATHERTIME - (GATHERTIME / 4.0f))
			{
				// 点滅
				m_pArea[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f));
			}
			// 一定時間経過したら
			if (m_nCntTime >= GATHERTIME - (GATHERTIME / 9.0f))
			{
				// 点滅
				m_pArea[nCnt]->SetBlink(true);
			}
		}
	}

	// カウンターが制限以上なら
	if (m_nCntTime >= GATHERTIME)
	{
		m_nCntTime = 0;	// カウンターをクリア

		// 範囲内のインタラクトを上昇
		CInteract::SetRising();

		// エリアのリセット
		ResetArea();

		// 新しいウェーブセット
		SetWave();

		// 音
		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_GATHER);

		return;
	}

	// デバッグ表示
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("範囲内数：%d", CGatherArea::GetAllIn());
	pDegub->Print("範囲内ゴミ数：%d", CGatherArea::GetDustIn());
	pDegub->Print("範囲内雑貨数：%d", CGatherArea::GetGoodsIn());
}
//===============================================
// 描画処理
//===============================================
void CGatherAreaManager::Draw(void)
{
}
//===============================================
// ウェーブを設定
//===============================================
void CGatherAreaManager::SetWave(void)
{
	bool bDecided = false;	// 決まったかどうか
	int nWave = 0;			// ウェーブ数

	// 決まるまでループ
	while (1)
	{
		// ウェーブをランダムで選択
		nWave = rand() % WAVE_MAX;

		// 現在のウェーブと同じなら
		if (nWave == (int)m_wave)
		{// 決まっていない
			bDecided = false;
		}
		// 現在のウェーブ以外なら
		else
		{// 決まった
			bDecided = true;
		}

		// 決まっていたら
		if (bDecided == true)
		{// ループを抜ける
			break;
		}
	}

	// 現在のウェーブを設定
	m_wave = (WAVE)nWave;

	// ウェーブ読み込み
	LoadWave();
}
//===============================================
// ウェーブを読み込み
//===============================================
void CGatherAreaManager::LoadWave(void)
{
	m_nNumArea = 0;		// 最大エリア数初期化

	// 読み込み情報格納用変数
	D3DXVECTOR3 pos = VECTOR3_NULL;		// 位置
	float fRadius = 0.0f;				// 半径

	// ファイルを開く
	std::ifstream File("data\\STAGE\\WAVE\\wave00.txt");

	// ファイルが開けたかチェック
	if (!File.is_open())
	{ // 開けなかった場合
		std::cerr << "ファイルを開けませんでした！" << std::endl;
		return;
	}

	std::string line;		// 一行全て格納する用の変数
	std::string head;		// １単語ずつ調べる用の変数
	
	bool inScript = false;	// スクリプト内にいるか
	bool inWaveSet = false;	// ウェーブセット内にいるか
	bool inAreaSet = false;	// エリアセット内にいるか
	bool bLoadWave = false;	// すでにウェーブを読み込んだか

	// 一行ずつ読み込む
	while (std::getline(File, line))
	{
		if (line.empty()) continue;		// 空ならスキップ
		if (line[0] == '#') continue;   // コメント行スキップ

		// 行をトークン化する(スペースごとに区切る・１単語ごとに区切る)
		std::istringstream iss(line);
		iss >> head;	// 最初の単語が入る

		//---------------------------------------------
		// SCRIPT
		//---------------------------------------------
		// SCRIPTが含まれていたら(find でSCRIPTを探す)
		if (head.find("SCRIPT") != std::string::npos)
		{
			inScript = true;
			continue;	// 再読み込み
		}
		// END_SCRIPTを読み取ったら
		else if (head.find("END_SCRIPT") != std::string::npos)
		{
			inScript = false;
			break;	// 終わる
		}
		// スクリプト内にないなら
		if (!inScript) continue;	// 再読み込み

		//---------------------------------------------
		// WAVE
		//---------------------------------------------
		// WAVESETが含まれていたら
		if (head.find("WAVESET") != std::string::npos)
		{
			// ウェーブをまだ読み込んでいないなら
			if (bLoadWave == false)
			{
				inWaveSet = true;	// ウェーブセット内にいる
			}
			// すでにウェーブを読み込んでいたら
			else
			{
				inWaveSet = false;	// ウェーブセット内にいない
			}
			continue;	// 再読み込み
		}
		// END_WAVESETを読み取ったら
		else if (head.find("END_WAVESET") != std::string::npos)
		{
			inWaveSet = false;	// ウェーブセット内にいない
			bLoadWave = true;	// すでにウェーブを読み込んだ
			continue;			// 再読み込み
		}
		// ウェーブセット内にないなら
		if (!inWaveSet) continue;	// 再読み込み

		// WAVEを読み取ったら
		if (head.find("WAVE") != std::string::npos)
		{
			char eq;	// '=' を読み飛ばす用変数
			int nWave;	// ウェーブ番号格納用

			iss >> eq >> nWave;	// ウェーブ番号読み込み

			// ウェーブ番号が読み込むウェーブと違うなら
			if (nWave != (int)m_wave)
			{
				inWaveSet = false;	// ウェーブセット内じゃなくする
				continue;			// 再読み込み
			}
		}

		//---------------------------------------------
		// AREA
		//---------------------------------------------
		// AREA_SETを読み取ったら
		if (head.find("AREA_SET") != std::string::npos)
		{
			inAreaSet = true;
		}
		// END_AREASETを読み取ったら
		else if (head.find("END_AREASET") != std::string::npos)
		{
			// エリア生成
			CGatherArea* area = CGatherArea::Create(pos, VECTOR3_NULL, fRadius, 25.0f);
			m_pArea.push_back(area);	// 配列の末尾に area のポインタを追加する
			inAreaSet = false;
			continue;	// 再読み込み
		}
		// エリアセット内にないなら
		if (!inAreaSet) continue;	// 再読み込み

		// POSを読み取ったら
		if (head.find("POS") != std::string::npos)
		{
			char eq;	// '=' を読み飛ばす用変数

			iss >> eq >> pos.x >> pos.y >> pos.z;  // 位置を読み込む
		}
		// RADIUSを読み取ったら
		else if (head.find("RADIUS") != std::string::npos)
		{
			char eq;	// '=' を読み飛ばす用変数

			iss >> eq >> fRadius;  // 半径を読み込む
		}
	}
	File.close(); // ファイルを閉じる

	// エリア数を取得
	m_nNumArea = m_pArea.size();
}
//===============================================
// エリアをリセット
//===============================================
void CGatherAreaManager::ResetArea(void)
{
	// 全エリア破棄
	for (auto* pArea : m_pArea)
	{
		pArea->SetRising(true);	// 上昇
		pArea = nullptr;		// NULLにする
	}
	m_pArea.clear();	// ポインタのリストをすべて削除
}
