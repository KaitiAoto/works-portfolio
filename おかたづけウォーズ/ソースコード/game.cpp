//===============================================
//
// ゲーム処理[game.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "interact.h"
#include "timer.h"
#include "flowerVase.h"
#include "gatherAreaManager.h"
#include "file.h"
#include "objectX.h"

// 静的メンバ変数
CPlayer* CGame::m_pPlayer = nullptr;
CObject3D* CGame::m_pField = nullptr;
CPause* CGame::m_pPause = nullptr;
CBoss* CGame::m_pBoss = nullptr;
CAbalogTimer* CGame::m_pTimer = nullptr;
CTutorial* CGame::m_pTutorial = nullptr;
CGame::MODE CGame::m_mode = CGame::MODE_NONE;
CGame::TYPE CGame::m_type = CGame::TYPE_TUTORIAL;
CLoadStage* CGame::m_pStage = nullptr;

//===============================================
// コンストラクタ
//===============================================
CGame::CGame(TYPE mode) :CScene(CScene::MODE_GAME)
{
	m_type = mode;
	m_pBoss = nullptr;
}
//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{
}
//===============================================
// 初期化
//===============================================
void CGame::Init(void)
{
	// ポーズ生成
	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	// ステージ読み込み生成
	if (m_pStage == nullptr)
	{
		m_pStage = new CLoadStage;

		m_pStage->Load("data\\STAGE\\model01.txt");
	}

	D3DXVECTOR3 FieldPos = VECTOR3_NULL;
	float fFieldHeight = 0.0f;
	// フィールドの情報取得
	if (m_pField != nullptr)
	{
		FieldPos = m_pField->GetPos();
		fFieldHeight = m_pField->GetHeight();
	}

	// プレイヤー
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create("data\\MOTION\\motionPlayer.txt", VECTOR3_NULL, VECTOR3_NULL);
	}
	if (m_pBoss == nullptr)
	{
		// ボス生成
		m_pBoss = CBoss::Create(D3DXVECTOR3(0.0f, 0.0f, FieldPos.z + fFieldHeight * 1.25f), VECTOR3_NULL);
	}

	if (m_type == TYPE_GAME)
	{
		// 花瓶
		CFlowerVase::Create(D3DXVECTOR3(450.0f, 0.0f, 450.0f), VECTOR3_NULL);
		CFlowerVase::Create(D3DXVECTOR3(-450.0f, 0.0f, 450.0f), VECTOR3_NULL);
		CFlowerVase::Create(D3DXVECTOR3(450.0f, 0.0f, -450.0f), VECTOR3_NULL);
		CFlowerVase::Create(D3DXVECTOR3(-450.0f, 0.0f, -450.0f), VECTOR3_NULL);

		// インタラクトオブジェクトランダム大量配置
		const int gridCountX = 15;		// X方向の数
		const int gridCountZ = 15;		// Z方向の数
		const float spacing = 60.0f;	// 配置間隔

		for (int x = -gridCountX / 2; x < gridCountX / 2; ++x)
		{
			for (int z = -gridCountZ / 2; z < gridCountZ / 2; ++z)
			{
				D3DXVECTOR3 pos(
					x * spacing,
					0.0f,
					z * spacing
				);

				// タイプをランダムで選択
				const int Max_Probability = 20;
				int nType = rand() % (Max_Probability + 1);
				CInteract::TYPE type;

				// ゴミ
				if (nType >= 0 && nType < Max_Probability / 2)
				{
					type = CInteract::TYPE_DUST;
				}
				// 雑貨
				else if (nType >= Max_Probability / 2 && nType < Max_Probability)
				{
					type = CInteract::TYPE_GOODS;
				}
				// 花
				else
				{
					type = CInteract::TYPE_FLOWER;
				}
				//CObjectPhysics::Create("data\\MODEL\\someBooks00.x", pos, D3DXVECTOR3(0, 0, 0));

				CInteract::Create(pos, VECTOR3_NULL, type);
			}
		}

		// 雑貨収集エリア
		//CGatherArea::Create(D3DXVECTOR3(0.0f, 0.5f, 0.0f), VECTOR3_NULL, 150.0f, 50.0f);

		// 雑貨収集エリアマネージャー生成
		CGatherAreaManager::Create();

		// タイマー生成
		m_pTimer = CAbalogTimer::Create(D3DXVECTOR3(0.0f, 1140.0f, -1000.0f), VECTOR3_NULL, GAME_TIME);
	}
	else if (m_type == TYPE_TUTORIAL)
	{
		CObject2D::Create("data\\TEXTURE\\skip00.png", D3DXVECTOR3(SCREEN_WIDTH / 1.1f, SCREEN_HEIGHT / 1.09f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(200.0f, 100.0f), 5);

		m_pTutorial = new CTutorial;
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Init();
		}
	}

	// ゲームモード設定
	SetMode(MODE_NONE);

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_GAME);
}
//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
{
	// ステージ破棄
	if (m_pStage != nullptr)
	{
		delete m_pStage;
		m_pStage = nullptr;
	}

	// ポーズ破棄
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	// プレイヤーをNULLに
	m_pPlayer = nullptr;

	m_pField = nullptr;

	if (m_type == TYPE_TUTORIAL && m_pTutorial != nullptr)
	{
		m_pTutorial->Uninit();
		delete m_pTutorial;
		m_pTutorial = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//===============================================
// 更新
//===============================================
void CGame::Update(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	CFade* pFade = CManager::GetFade();

	if (m_type == TYPE_GAME)
	{
		if (m_mode == MODE_FIN)
		{
			// スコアを算出して保存
			CalcScore();

			// データを保存
			m_pBoss->EndSave();

			pFade->Set(CScene::MODE_RESULT);	// リザルトへフェード
			return;
		}
		else if (m_mode == MODE_RESET)
		{
			pFade->Set(CScene::MODE_GAME);		// ゲームへフェード
			return;
		}
		else if (m_mode == MODE_BACK)
		{
			pFade->Set(CScene::MODE_TITLE);		// タイトルへフェード
			return;
		}
		else if (m_mode == MODE_PLAY)
		{
		}
	}
	else
	{
		if (pInputKey->GetTrigger(DIK_SPACE) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_BACK) == true)
		{
			CGame::SetMode(CGame::MODE_FIN);
			return;
		}

		if (m_mode == MODE_FIN)
		{
			pFade->Set(CScene::MODE_GAME);		// ゲームへフェード
			return;
		}
		else if (m_mode == MODE_RESET)
		{
			pFade->Set(CScene::MODE_TUTORIAL);	// チュートリアルへフェード
			return;
		}
		else if (m_mode == MODE_BACK)
		{
			pFade->Set(CScene::MODE_TITLE);		// タイトルへフェード
			return;
		}
		else
		{
			if (m_pTutorial != nullptr)
			{
				m_pTutorial->Update();
			}
		}
	}

	if (pInputKey->GetTrigger(DIK_TAB) == true || pInputKey->GetTrigger(DIK_P) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_START) == true)
	{
		CManager::SetPause(true);
	}

#ifdef _DEBUG

	if (pInputKey->GetTrigger(DIK_F3) == true)
	{
		m_mode = MODE_FIN;
		return;
	}
#endif

}
//===============================================
// 描画
//===============================================
void CGame::Draw(void)
{
	if (m_type == TYPE_TUTORIAL)
	{
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Draw();
		}
	}
}
//===============================================
// スコアを算出して保存
//===============================================
void CGame::CalcScore(void)
{
	int nScore = 0;	// スコア代入用変数

	// プラス要素代入用変数
	int nBossHP = m_pBoss->GetMaxLife() - m_pBoss->GetLife();	// ボスのHP(最大HPから今のHPを引いた値)
	int nPlayerHP = m_pPlayer->GetLife();						// プレイヤーのHP
	int nBouqetCnt = CFlowerVase::GetNumBouqet();				// 花束を飾った回数

	// マイナス要素代入用変数
	int nMissCnt = m_pBoss->GetAI()->GetNumMiss();				// ミス数
	int nAttackCnt = m_pBoss->GetAI()->GetNumAttack();			// 攻撃数

	// マイナス要素算出
	float fMinus = 1.0f + (float)(nMissCnt * 0.03f) + (float)(nAttackCnt * 0.05f);

	// プラス要素からスコアを算出
	nScore = (nBossHP * 500000) + (nPlayerHP * 200000) + (nBouqetCnt * 10000);

	// 算出したスコアをマイナス要素で除算
	nScore /= (int)fMinus;
	nScore -= nMissCnt + nAttackCnt;

	std::vector<int> Score;
	Score.push_back(nScore);
	// スコアを保存
	CFile::IntSave("data\\TEXT\\NowScore.txt", Score);
}