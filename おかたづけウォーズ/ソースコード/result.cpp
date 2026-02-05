//===============================================
//
// リザルト処理[result.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "ranking.h"

//===============================================
// コンストラクタ
//===============================================
CResult::CResult():CScene(CScene::MODE_RESULT)
{
	// 値をクリア
	m_nCntStay = 0;
}
//===============================================
// デストラクタ
//===============================================
CResult::~CResult()
{
}
//===============================================
// 初期化
//===============================================
void CResult::Init(void)
{
	// 待機時間代入
	m_nCntStay = RESULT_STAY;

	// 背景生成
	CObject2D::Create("data\\TEXTURE\\result01.jpg", D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 5);

	std::vector<int> data = CFile::IntLoad("data\\TEXT\\ResultSave.txt");

	int nLife = 0;
	int nNumMiss = 0;
	int nNumAttack = 0;

	if (data.size() >= 3)
	{
		nLife = data[0];
		nNumMiss = data[1];
		nNumAttack = data[2];
	}

	// ゲージ生成
	if (m_pGauge == nullptr)
	{
		m_pGauge = CGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 8.0f, 225.0f, 0.0f), (float)CBoss::GetMaxLife(), D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f), D3DXVECTOR2(18.0f, 25.0f));
		m_pGauge->Set((float)nLife, D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f));	// 今回のHPにする
		m_pGauge->CreateFrame("data\\TEXTURE\\gauge_frame00.png");
		m_pGauge->CreateBack("data\\TEXTURE\\gauge00.jpeg", D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));
		// アイコン
		CObject2D::Create("data\\TEXTURE\\mood00.png", D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 175, 0.0f), VECTOR3_NULL, D3DXVECTOR2(150.0f, 75.0f), 9);
	}

	// ミス数生成
	m_pMissCnt = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 1.75f, 0.0f), SCORE_SIZE * 2.0f, SCORE_SIZE * 2.0f, 3, 100);
	m_pMissCnt->SetScore(nNumMiss);	// 今回のミス数にする
	// アイコン
	CObject2D::Create("data\\TEXTURE\\missCnt00.png", D3DXVECTOR3(SCREEN_WIDTH / 3.5f, SCREEN_HEIGHT / 2.25f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(300.0f, 75.0f), 7);

	// 攻撃数生成
	m_pAttackCnt = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 1.75f, 0.0f), SCORE_SIZE * 2.0f, SCORE_SIZE * 2.0f, 2, 10);
	m_pAttackCnt->SetScore(nNumAttack);
	// アイコン
	CObject2D::Create("data\\TEXTURE\\attackCnt00.png", D3DXVECTOR3(SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 2.25f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(300.0f, 75.0f), 7);

	// 総スコア生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 1.25f, 0.0f), SCORE_SIZE * 3.0f, SCORE_SIZE * 3.0f);
	std::vector<int> Score = CFile::IntLoad("data\\TEXT\\NowScore.txt");
	m_pScore->SetScore(Score[0]);

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_RESULT);
}
//===============================================
// 終了処理
//===============================================
void CResult::Uninit(void)
{
	// オブジェクトの破棄
	CObject::Release();
}
//===============================================
// 更新
//===============================================
void CResult::Update(void)
{
	// 待機時間減算
	m_nCntStay--;
	// 待機時間が０になったら
	if (m_nCntStay <= 0)
	{
		// 待機時間を０に
		m_nCntStay = 0;

		// キー取得
		CInputKey* pInputKey = CManager::GetInputKey();
		// マウス取得
		CInputMouse* pInputMouse = CManager::GetInputMouse();
		// パッド
		CInputPad* pPad = CManager::GetInputPad();
		// 決定キーが押されたら
		if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(0) == true || pPad->GetTrigger(CInputPad::JOYKEY_A) == true)
		{
			// ランキングへ移行
			CFade* pFade = CManager::GetFade();
			pFade->Set(CScene::MODE_RANKING);
		}
	}
}
//===============================================
// 描画
//===============================================
void CResult::Draw(void)
{
}