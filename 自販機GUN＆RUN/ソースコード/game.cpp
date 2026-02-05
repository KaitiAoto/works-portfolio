//================================
//
// ゲーム処理[game.cpp]
// Author : Kaiti Aoto
//
//================================
#include "game.h"
#include "manager.h"
#include "fade.h"

//静的メンバ変数
CPlayer* CGame::m_pPlayer = nullptr;
CTimerMana* CGame::m_pTimer = nullptr;
CObject3D* CGame::m_pObj3D = nullptr;
CScoreMana* CGame::m_pBreakCnt = nullptr;
CScoreMana* CGame::m_pTotalScore = nullptr;
CCollision* CGame::m_pColl = nullptr;
CMatchUp* CGame::m_pMatchup = nullptr;
CHpGauge* CGame::m_pHpGauge = nullptr;
CLoadStage* CGame::m_pStage = nullptr;
CStart* CGame::m_pStart = nullptr;
CResetGauge* CGame::m_pReset = nullptr;
CMap* CGame::m_pMap = nullptr;
CBuff* CGame::m_pBuff = nullptr;
CScreenFlash* CGame::m_pScreenFlash = nullptr;
CTutorial* CGame::m_pTutorial = nullptr;
CTutorialConveni* CGame::m_pTutoConveni = nullptr;
CPause* CGame::m_pPause = nullptr;
CSell* CGame::m_pSell = nullptr;

CGame::MODE CGame::m_mode = CGame::MODE_NONE;
//==================
// コンストラクタ
//==================
CGame::CGame():CScene(CScene::MODE_GAME)
{
}
//================
// デストラクタ
//================
CGame::~CGame()
{
}
//=======================
// 初期化
//=======================
void CGame::Init(void)
{
	//当たり判定生成
	if (m_pColl == nullptr)
	{
		m_pColl = new CCollision;
	}

	//相性生成
	if (m_pMatchup == nullptr)
	{
		m_pMatchup = new CMatchUp;
	}


	//タイマー生成
	if (m_pTimer == nullptr)
	{
		m_pTimer = CTimerMana::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - (TIMER_SIZE * 4.5), 50.0f, 0.0f));
	}

	const float ScoreX = 950.0f;
	//スコア生成
	if (m_pBreakCnt == nullptr)
	{
		m_pBreakCnt = CScoreMana::Create(D3DXVECTOR3(ScoreX, 90.0f, 0.0f), SCORE_SIZE, SCORE_SIZE, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 2, CScoreMana::TYPE_CNTDOWN);
		CObject2D::Create("data\\TEXTURE\\conveni_icon00.png", D3DXVECTOR3(ScoreX - (SCORE_SIZE * 2), 90.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCORE_SIZE * 2, SCORE_SIZE * 2, 8);
	}
	//スコア生成
	if (m_pTotalScore == nullptr)
	{
		m_pTotalScore = CScoreMana::Create(D3DXVECTOR3(ScoreX, 40.0f, 0.0f), SCORE_SIZE, SCORE_SIZE, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 6);
		CObject2D::Create("data\\TEXTURE\\icon_star.png", D3DXVECTOR3(ScoreX - (SCORE_SIZE * 2), 40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCORE_SIZE * 2, SCORE_SIZE * 2, 8);
	}

	if (m_pHpGauge == nullptr)
	{
		m_pHpGauge = CHpGauge::Create(D3DXVECTOR3(50.0f, 40.0f, 0.0f), PLAYER_LIFE, GAUGE_Y, D3DCOLOR_RGBA(1, 255, 1, 255));
	}

	if (m_pReset == nullptr)
	{
		m_pReset = CResetGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - (BULLET_COUNT_SIZE * 5), SCREEN_HEIGHT / 1.35f - 50.0f, 0.0f), 0, GAUGE_Y, D3DCOLOR_RGBA(255, 255, 1, 255));
	}

	if (m_pBuff == nullptr)
	{
		m_pBuff = new CBuff;
	}

	m_pMap = CMap::Create(D3DXVECTOR3(1100.0f, 550.0f, 0.0f), 280.0f, 280.0f);

	if (m_pStage == nullptr)
	{
		m_pStage = new CLoadStage;

		m_pStage->Load("data\\STAGE\\model01.txt");
	}

	//プレイヤー
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create("data\\STAGE\\motion_vending.txt", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	m_pStart = CStart::Create("data\\MODEL\\barricade00.x", D3DXVECTOR3(-870.0f, 0.0f, -880.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f));
	
	CEnemyGroup::Create(D3DXVECTOR3(-150.0f, 0.0f, -550.0f));
	CEnemyGroup::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));
	CEnemyGroup::Create(D3DXVECTOR3(-170.0f, 0.0f, 550.0f));
	CEnemyGroup::Create(D3DXVECTOR3(560.0f, 0.0f, -100.0f));
	CEnemyGroup::Create(D3DXVECTOR3(300.0f, 0.0f, -700.0f));
	CEnemyGroup::Create(D3DXVECTOR3(800.0f, 0.0f, 700.0f));

	CObject3D::Create("data\\TEXTURE\\field00.jpeg", D3DXVECTOR3(0.0f, -0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000, 3000, CObject3D::TYPE_FIELD);
	CMeshCylinder::Create("data\\TEXTURE\\city00.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2900, 200, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), CMeshCylinder::TYPE_IN);
	CMeshSphere::Create("data\\TEXTURE\\sky001.jpg", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000, 3000, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), CMeshSphere::TYPE_HALF_TOP);

	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	m_pTutorial = CTutorial::Create(D3DXVECTOR3(TUTORIAL_SIZE / 1.5f, SCREEN_HEIGHT - (TUTORIAL_SIZE / 1.5f), 0.0f), TUTORIAL_SIZE, TUTORIAL_SIZE);

	CStartUI::Create("data\\TEXTURE\\tutorial_start00.png", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), STARTUI_SIZE_X, STARTUI_SIZE_Y);
	
	m_pScreenFlash = CScreenFlash::Create(nullptr, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	if (m_pSell == nullptr)
	{
		m_pSell = new CSell;
		m_pSell->Init();
	}

	SetMode(MODE_TUTORIAL);

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_GAME);
}
//=============
// 終了処理
//=============
void CGame::Uninit(void)
{
	if (m_pStage != nullptr)
	{
		m_pStage->Unload();
		delete m_pStage;
		m_pStage = nullptr;
	}

	//
	if (m_pHpGauge != nullptr)
	{
		m_pHpGauge = nullptr;
	}

	////スコア破棄
	//if (m_pRestock != NULL)
	//{
	//	m_pRestock = NULL;
	//}

	//スコア破棄
	if (m_pBreakCnt != nullptr)
	{
		//スコア終了処理
		m_pBreakCnt->Uninit();

		delete m_pBreakCnt;
		m_pBreakCnt = nullptr;
	}
	//スコア破棄
	if (m_pTotalScore != nullptr)
	{
		//スコア終了処理
		m_pTotalScore->Uninit();

		delete m_pTotalScore;
		m_pTotalScore = nullptr;
	}

	//タイマー破棄
	if (m_pTimer != nullptr)
	{
		//タイマー終了処理
		m_pTimer->Uninit();

		delete m_pTimer;
		m_pTimer = nullptr;
	}

	//当たり判定
	if (m_pColl != nullptr)
	{
		delete m_pColl;
		m_pColl = nullptr;
	}

	//相性マネージャー
	if (m_pMatchup != nullptr)
	{
		delete m_pMatchup;
		m_pMatchup = nullptr;
	}

	if (m_pReset != nullptr)
	{
		m_pReset = nullptr;
	}

	//
	if (m_pBuff != nullptr)
	{
		delete m_pBuff;
		m_pBuff = nullptr;
	}

	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = nullptr;
	}

	if (m_pSell != nullptr)
	{
		delete m_pSell;
		m_pSell = nullptr;
	}


	CObject::Release();
}
//=======================
// マネージャーの更新
//=======================
void CGame::Update(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pInputPad = CManager::GetInputPad();

	CFade* pFade = CManager::GetFade();

	if (m_mode == MODE_FIN)
	{
		pFade->Set(CScene::MODE_RESULT);
		return;
	}
	else if (m_mode == MODE_RESET)
	{
		pFade->Set(CScene::MODE_GAME);
		return;
	}
	else if (m_mode == MODE_BACK)
	{
		pFade->Set(CScene::MODE_TITLE);
		return;
	}
	else if (m_mode == MODE_PLAY)
	{
		bool bStart = m_pStart->GetStart();
		if (bStart == true)
		{
			if (m_pTimer != nullptr)
			{
				//タイマー更新
				m_pTimer->Update();
			}
		}
		if (m_pHpGauge != nullptr)
		{
			//
			m_pHpGauge->Update();
		}
		if (m_pBuff != nullptr)
		{
			m_pBuff->Update(1.0f / 60.0f);
		}

		m_pSell->Update();
	}
	if (m_pBreakCnt != nullptr)
	{
		m_pBreakCnt->Update();
		if (m_pBreakCnt->GetScore() <= 0)
		{
			SetMode(MODE_FIN);
		}

	}
	if (m_pTotalScore != nullptr)
	{
		m_pTotalScore->Update();
	}

	if (pInputKey->GetTrigger(DIK_TAB) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_START) == true)
	{
		CManager::SetPause(true);
	}

	if (pInputKey->GetTrigger(DIK_F3) == true)
	{
		pFade->Set(CScene::MODE_RANKING);
		return;
	}

}
//=======================
// マネージャーの描画
//=======================
void CGame::Draw(void)
{

}