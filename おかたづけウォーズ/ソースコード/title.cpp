//===============================================
//
// タイトル処理[title.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "title.h"
#include "manager.h"
#include "object2D.h"
#include "title_select.h"

// 静的メンバ変数
CLoadStage* CTitle::m_pStage = nullptr;
CTitleLogo* CTitle::m_pLogo = nullptr;

//===============================================
// コンストラクタ
//===============================================
CTitle::CTitle():CScene(CScene::MODE_TITLE)
{
	
}
//===============================================
// デストラクタ
//===============================================
CTitle::~CTitle()
{
}
//===============================================
// 初期化
//===============================================
void CTitle::Init(void)
{
	CObject2D::Create("data\\TEXTURE\\Back01.jpg", D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 5);

	// ロゴ生成
	m_pLogo = CTitleLogo::Create("data\\TEXTURE\\titlelogo02.png", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3.5f, 0.0f), LOGO_SIZE_X, LOGO_SIZE_Y);

	CTitleSelect::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.5f, 0.0f));

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_TITLE);
}
//===============================================
// 終了処理
//===============================================
void CTitle::Uninit(void)
{
	CObject::Release();
}
//===============================================
// 更新
//===============================================
void CTitle::Update(void)
{

}
//===============================================
// 描画
//===============================================
void CTitle::Draw(void)
{

}