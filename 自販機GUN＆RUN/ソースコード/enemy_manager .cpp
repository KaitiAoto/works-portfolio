//=====================================
//
// 敵マネージャー[enemy_manager.cpp]
// Author:kaiti
//
//=====================================
#include "enemy_manager.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
//静的メンバ変数
CEnemyMana* CEnemyMana::m_pEnemyMane = NULL;
int CEnemyMana::m_nCntEnemy = 0;
//==================
// コンストラクタ
//==================
CEnemyMana::CEnemyMana()
{
	m_nCntSpan = 0;
}
//================
// デストラクタ
//================
CEnemyMana::~CEnemyMana()
{
}
//===========
// 生成処理
//===========
CEnemyMana* CEnemyMana::Create()
{
	////敵
	//CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, SCREEN_HEIGHT / 5, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::TYPE_A);
	//CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::TYPE_B);
	//CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, SCREEN_HEIGHT / 5, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::TYPE_C);

	return m_pEnemyMane;
}
//===============
// 初期化処理
//===============
HRESULT CEnemyMana::Init(void)
{
	//初期化
	//m_nCntSpan = ENEMY_SPAN;

	return S_OK;
}
//============
// 終了処理
//============
void CEnemyMana::Uninit(void)
{

}
//============
// 更新処理
//============
void CEnemyMana::Update(void)
{

	m_nCntSpan--;

	if (m_nCntSpan <= 0)
	{
		//m_nCntSpan = ENEMY_SPAN;

		//CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, SCREEN_HEIGHT / 5, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEnemy::TYPE_A);
	}
}
//============
// 描画処理
//============
void CEnemyMana::Draw(void)
{
}
//=============
// 設定処理
//=============
void CEnemyMana::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

}