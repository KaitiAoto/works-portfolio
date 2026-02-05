//==============================
//
// 敵拠点[enemy_base.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "enemy_group.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CEnemyGroup::CEnemyGroup(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bUse = true;

	m_nRespawn = 0;

	m_bRespawn = false;
}
//================
// デストラクタ
//================
CEnemyGroup::~CEnemyGroup()
{

}
//===========
// 生成処理
//===========
CEnemyGroup* CEnemyGroup::Create(D3DXVECTOR3 pos)
{
	CEnemyGroup* pEnemyBase = new CEnemyGroup;
	if (!pEnemyBase)
		return nullptr;

	if (FAILED(pEnemyBase->Init(pos)))
	{
		delete pEnemyBase;
		return nullptr;
	}

	return pEnemyBase;
}
//===============
// 初期化処理
//===============
HRESULT CEnemyGroup::Init(const D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_bUse = true;
	m_nRespawn = 0;

	CreateEnemy();
#ifdef _DEBUG
	//CMeshCylinder::Create(nullptr, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), { 0.0f,0.0f,0.0f }, 20.0f, 100, D3DXCOLOR(1.0, 1.0, 0.0, 0.75), CMeshCylinder::TYPE_BOTHSIDES);
#endif // DEBUG


	//オブジェクトの種類設定
	SetObjType(TYPE_FIELD);
	
	return S_OK;
}
//============
// 終了処理
//============
void CEnemyGroup::Uninit(void)
{
	CObject::Release();
}
//============
// 更新処理
//============
void CEnemyGroup::Update(void)
{
	if (m_bUse == true)
	{
		if (m_MyEnemy <= 0)
		{
			m_MyEnemy = 0;
			m_bUse = false;
		}
	}
	else if(m_bUse == false)
	{
		m_nRespawn++;
		if (m_nRespawn >= ENEMYGROUP_RESPAWN)
		{
			m_nRespawn = 0;
			CreateEnemy();
			m_bUse = true;
		}
	}
}
//============
// 描画処理
//============
void CEnemyGroup::Draw(void)
{
	
}
//=============
// 敵生成処理
//=============
void CEnemyGroup::CreateEnemy(void)
{
	auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	size_t seed = static_cast<size_t>(now) ^ reinterpret_cast<size_t>(this);

	std::mt19937 mt((unsigned int)seed);
	std::uniform_int_distribution<int> distType(0, CEnemy::TYPE_MAX - 1);

	const int Max = 2;
	const int Min = 1;
	std::uniform_int_distribution<int> distCount(Min, Max);

	int nCntEnemy = distCount(mt);
	m_MyEnemy = nCntEnemy;

	for (int nCnt = 0; nCnt < nCntEnemy; nCnt++)
	{
		int nType = distType(mt);
		CEnemy::Create(
			m_pos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			(CEnemy::TYPE)nType, this);
	}
}