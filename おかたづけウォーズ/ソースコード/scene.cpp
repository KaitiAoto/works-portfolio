//===============================================
//
// シーン処理[scene.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "scene.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"
#include "tutorial.h"

// 静的メンバ変数
CScene::MODE CScene::m_mode = CScene::MODE_TITLE;

//===============================================
// コンストラクタ
//===============================================
CScene::CScene(MODE mode):CObject(0)
{
	m_mode = mode;
}

//===============================================
// デストラクタ
//===============================================
CScene::~CScene()
{

}
//===============================================
// 生成処理
//===============================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = nullptr;
	switch (mode)
	{
	case MODE_TITLE:
		pScene = new CTitle;
		break;
	case MODE_TUTORIAL:
		pScene = new CGame(CGame::TYPE_TUTORIAL);
		break;
	case MODE_GAME:
		pScene = new CGame(CGame::TYPE_GAME);
		break;
	case MODE_RESULT:
		pScene = new CResult;
		break;
	case MODE_RANKING:
		pScene = new CRanking;
		break;

	default:
		break;
	}

	//if (pScene != nullptr)
	//{
	//	pScene->Init();
	//}

	m_mode = mode;

	return pScene;
}
