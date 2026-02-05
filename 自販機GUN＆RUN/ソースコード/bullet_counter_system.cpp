//============================================
//
// 弾数カウンター[bullet_counter.cpp]
// Author : Kaiti Aoto
//
//============================================

// インクルード
#include "bullet_counter_system.h"
#include "renderer.h"
#include "manager.h"
#include "timer_manager.h"

//==================
// コンストラクタ
//==================
CBullerCntSystem::CBullerCntSystem(int nPriority) :CObject(nPriority)
{
	// 値をクリア
}
//================
// デストラクタ
//================
CBullerCntSystem::~CBullerCntSystem()
{
}
//===========
// 生成処理
//===========
CBullerCntSystem* CBullerCntSystem::Create(void)
{
	// 生成
	CBullerCntSystem* pBullerCounter = new CBullerCntSystem;
	// 初期化
	if (FAILED(pBullerCounter->Init()))
	{// NULLチェック
		delete pBullerCounter;
		return nullptr;
	}

	return pBullerCounter;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBullerCntSystem::Init(void)
{
	m_bWhich = true;
	m_bMove = false;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_MainPos = D3DXVECTOR3(SCREEN_WIDTH / 2 + (BULLET_COUNT_SIZE * 1.0f), SCREEN_HEIGHT / 1.35f, 0.0f);
	m_SubPos = D3DXVECTOR3(SCREEN_WIDTH / 2 + (BULLET_COUNT_SIZE * 1.0f), SCREEN_HEIGHT / 1.15f, 0.0f);

	//弾数カウント生成
	if (m_pBulletCount == nullptr)
	{
		m_pBulletCount = CBulletCntMana::Create(m_MainPos, false);
	}
	//弾数カウント生成
	if (m_pSubBulletCount == nullptr)
	{
		m_pSubBulletCount = CBulletCntMana::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + (BULLET_COUNT_SIZE * 1.0f), SCREEN_HEIGHT / 1.15f, 0.0f), true);
	}

	// オブジェクトの種類設定
	SetObjType(TYPE_BULLETCNT);

	return S_OK;
}
//============
// 終了処理
//============
void CBullerCntSystem::Uninit(void)
{
	//弾数カウンター破棄
	if (m_pBulletCount != nullptr)
	{
		//弾数カウンター終了処理
		m_pBulletCount->Uninit();

		delete m_pBulletCount;
		m_pBulletCount = nullptr;
	}

	//弾数カウンター破棄
	if (m_pSubBulletCount != nullptr)
	{
		//弾数カウンター終了処理
		m_pSubBulletCount->Uninit();

		delete m_pSubBulletCount;
		m_pSubBulletCount = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============
// 更新処理
//============
void CBullerCntSystem::Update(void)
{
	if (m_pBulletCount != nullptr)
	{
		//弾数カウント更新
		m_pBulletCount->Update();
	}
	if (m_pSubBulletCount != nullptr)
	{
		//弾数カウント更新
		m_pSubBulletCount->Update();
	}

	Move();
	Blink();
}
void CBullerCntSystem::Move(void)
{
	if (m_bMove == true)
	{
		if (m_bWhich == true)
		{
			m_pBulletCount->BecomeMain();
			m_pSubBulletCount->BecomeSub();
		}
		else
		{
			m_pBulletCount->BecomeSub();
			m_pSubBulletCount->BecomeMain();
		}
	}
}

void CBullerCntSystem::Blink(void)
{
	if (m_bBlink == true)
	{
		if (m_bRLShake == true)
		{
			m_rot.z += 0.1f;
			if (m_rot.z >= 0.3f)
			{
				m_bRLShake = false;
			}
		}
		else if (m_bRLShake == false)
		{
			m_rot.z -= 0.1f;
			if (m_rot.z <= -0.3f)
			{
				m_bRLShake = true;
			}
		}

		m_nCntBlink++;
		if (m_nCntBlink >= 30)
		{
			m_bBlink = false;
			m_nCntBlink = 0;

			m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		GetUseCnt()->Set(m_MainPos, m_rot);
	}
}

//============
// 描画処理
//============
void CBullerCntSystem::Draw(void)
{

}

void CBullerCntSystem::Change(void)
{
	m_bWhich = !m_bWhich;

	CPlayer* pPlayer = CGame::GetPlayer();
	CBulletCntMana* pCnt = GetUseCnt();

	pPlayer->SetContents(pCnt->GetContents());
	pPlayer->SetBulletType(pCnt->GetType());	

	m_bMove = true;
}

void CBullerCntSystem::Reset(void)
{
	m_pBulletCount->ResetCnt();
	m_pSubBulletCount->ResetCnt();
}

CBulletCntMana* CBullerCntSystem::GetUseCnt(void)
{
	if (m_bWhich == true)
	{
		return m_pBulletCount;
	}
	else
	{
		return m_pSubBulletCount;
	}
}

