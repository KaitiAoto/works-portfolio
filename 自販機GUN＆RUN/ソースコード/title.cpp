//================================
//
// タイトル処理[title.cpp]
// Author : Kaiti Aoto
//
//================================
#include "title.h"
#include "manager.h"
#include "object2D.h"
#include "title_select.h"
#include "click.h"

CLoadStage* CTitle::m_pStage = NULL;
CTitleLogo* CTitle::m_pLogo = NULL;

//==================
// コンストラクタ
//==================
CTitle::CTitle():CScene(CScene::MODE_TITLE)
{
	
}
//================
// デストラクタ
//================
CTitle::~CTitle()
{
}
//=======================
// 初期化
//=======================
void CTitle::Init(void)
{
	CManager::GetLight()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_pLogo = CTitleLogo::Create("data\\TEXTURE\\title01.png", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.5f, 0.0f), LOGO_SIZE_X, LOGO_SIZE_Y);

	CTitleSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, 0.0f));

	CClick::Create("data\\TEXTURE\\next00.png", D3DXVECTOR3(SCREEN_WIDTH - (CLICK_SIZE_X / 1.5f), SCREEN_HEIGHT  - (CLICK_SIZE_Y  / 1.5f), 0.0f), CLICK_SIZE_X, CLICK_SIZE_Y);

	if (m_pStage == NULL)
	{
		m_pStage = new CLoadStage;

		m_pStage->Load("data\\STAGE\\model01.txt");
	}
	CObject3D::Create("data\\TEXTURE\\field00.jpeg", D3DXVECTOR3(0.0f, -0.1f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000, 3000, CObject3D::TYPE_FIELD);
	CMeshCylinder::Create("data\\TEXTURE\\city00.png", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2900, 200, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), CMeshCylinder::TYPE_IN);
	CMeshSphere::Create("data\\TEXTURE\\sky001.jpg", D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000, 3000, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), CMeshSphere::TYPE_HALF_TOP);

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
}
//=============
// 終了処理
//=============
void CTitle::Uninit(void)
{
	if (m_pStage != NULL)
	{
		m_pStage->Unload();
		delete m_pStage;
		m_pStage = nullptr;
	}

	CObject::Release();
}
//=======================
// 更新
//=======================
void CTitle::Update(void)
{
	////キー取得
	//CInputKey* pInputKey = CManager::GetInputKey();
	////マウス取得
	//CInputMouse* pInputMouse = CManager::GetInputMouse();

	//if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(0) == true)
	//{
	//	CFade* pFade = CManager::GetFade();

	//	pFade->Set(CScene::MODE_GAME);
	//}
}
//=======================
// 描画
//=======================
void CTitle::Draw(void)
{

}