//================================
//
// シーン処理[scene.cpp]
// Author : Kaiti Aoto
//
//================================
#include "scene.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "ranking.h"

//==================
// コンストラクタ
//==================
CScene::CScene(MODE mode):CObject(0)
{
	m_mode = mode;
}

//==================
// デストラクタ
//==================
CScene::~CScene()
{

}
//============
// 生成処理
//============
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = nullptr;
	switch (mode)
	{
	case MODE_TITLE:
		pScene = new CTitle;
		break;
	case MODE_GAME:
		pScene = new CGame;
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

	return pScene;
}
