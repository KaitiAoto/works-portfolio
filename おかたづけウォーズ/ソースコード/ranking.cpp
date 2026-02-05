//===============================================
//
// ランキング処理[ranking.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "ranking.h"
#include "manager.h"
#include "object2D.h"

// 静的メンバ変数
CLoadStage* CRanking::m_pStage = nullptr;
CRankMana* CRanking::m_pRank = nullptr;

//===============================================
// コンストラクタ
//===============================================
CRanking::CRanking():CScene(CScene::MODE_RANKING)
{
	m_nCntStay = 0;
}
//===============================================
// デストラクタ
//===============================================
CRanking::~CRanking()
{
}
//===============================================
// 初期化
//===============================================
void CRanking::Init(void)
{
	m_nCntStay = RANK_STAY;

	m_pRank = CRankMana::Create(D3DXVECTOR3(SCREEN_WIDTH / 3.0f, SCREEN_HEIGHT / 3.5f, 0.0f), "data\\TEXT\\Ranking.txt");
	std::vector<int> Score = CFile::IntLoad("data\\TEXT\\NowScore.txt");

	m_pRank->Set(Score[0]);

	CObject2D::Create("data\\TEXTURE\\result00.jpg", D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 5);

	CObject3D::Create("data\\TEXTURE\\field00.jpeg", D3DXVECTOR3(0.0f, -0.1f, 0.0f), VECTOR3_NULL, 3000, 3000, CObject3D::TYPE_FIELD);
}
//===============================================
// 終了処理
//===============================================
void CRanking::Uninit(void)
{
	if (m_pRank != nullptr)
	{
		m_pRank->Uninit();
		delete m_pRank;
		m_pRank = nullptr;
	}

	CObject::Release();
}
//===============================================
// 更新
//===============================================
void CRanking::Update(void)
{
	m_pRank->Update();

	m_nCntStay--;
	if (m_nCntStay <= 0)
	{
		m_nCntStay = 0;
		// キー取得
		CInputKey* pInputKey = CManager::GetInputKey();
		// マウス取得
		CInputMouse* pInputMouse = CManager::GetInputMouse();
		// パッド
		CInputPad* pPad = CManager::GetInputPad();

		if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(0) == true || pPad->GetTrigger(CInputPad::JOYKEY_A) == true)
		{
			CFade* pFade = CManager::GetFade();

			pFade->Set(CScene::MODE_TITLE);
		}
	}
}
//===============================================
// 描画
//===============================================
void CRanking::Draw(void)
{

}