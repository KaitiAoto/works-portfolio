//================================================================
//
// 弾数カウンターマネージャー[bullet_counter_manager.cpp]
// Author : Kaiti Aoto
//
//================================================================

// インクルード
#include "bullet_counter_manager.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "player.h"

//
D3DXVECTOR3 CBulletCntMana::m_MainPos = {};
D3DXVECTOR3 CBulletCntMana::m_SubPos = {};

//==================
// コンストラクタ
//==================
CBulletCntMana::CBulletCntMana()
{
}
//================
// デストラクタ
//================
CBulletCntMana::~CBulletCntMana()
{
}
//===========
// 生成処理
//===========
CBulletCntMana* CBulletCntMana::Create(D3DXVECTOR3 pos, bool bSub)
{
	// 生成
	CBulletCntMana* pBulletCntMana = new CBulletCntMana;
	// 初期化
	if (FAILED(pBulletCntMana->Init(pos,bSub)))
	{// NULLチェック
		delete pBulletCntMana;
		return nullptr;
	}
	return pBulletCntMana;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBulletCntMana::Init(D3DXVECTOR3 pos, bool bSub)
{
	m_pos = pos;
	const float fDiv = 1.75f;
	m_fIconSize = BULLETICON_SIZE;
	m_fCntSize = BULLET_COUNT_SIZE;
	if (bSub == true)
	{
		m_fIconSize /= fDiv;
		m_fCntSize /= fDiv;

		m_SubPos = pos;
	}
	else if(bSub == false)
	{
		m_MainPos = pos;
	}

	// 弾アイコン生成
	m_pIcon = CBulletIcon::Create(D3DXVECTOR3(pos.x - (m_fCntSize * 4.5f), pos.y, 0.0f), m_fIconSize, m_fIconSize);

	// ×生成
	m_cross = CObject2D::Create("data\\TEXTURE\\multiply00.png", D3DXVECTOR3(pos.x - (m_fCntSize * 2.0f), pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fIconSize, m_fIconSize, 8);

	// 桁数分カウンター生成
	for (int nCnt = 0; nCnt < MAX_BULLETCNT; nCnt++)
	{
		m_pCounter[nCnt] = CBullerCounter::Create(D3DXVECTOR3(pos.x + (nCnt * m_fCntSize * 2.5f), pos.y, 0.0f), m_fCntSize, m_fCntSize);
	}

	return S_OK;
}
//============
// 終了処理
//============
void CBulletCntMana::Uninit(void)
{
}
//============
// 更新処理
//============
void CBulletCntMana::Update(void)
{	
	// カウンター更新
	UpdateCounter();

	m_pIcon->SetType(m_Bullet);
	m_pIcon->SetContens(m_nCount);
}
void CBulletCntMana::BecomeMain()
{
	const float SubPosY = 5.0f;
	const float AddSize = 1.0f;
	const float fDiv = 1.75f;

	static bool bSize = false;
	static bool bPos = false;

	m_pos.y -= SubPosY;
	m_fIconSize += AddSize;
	m_fCntSize += AddSize;
	if (m_fCntSize >= BULLET_COUNT_SIZE)
	{
		m_fCntSize = BULLET_COUNT_SIZE;
		m_fIconSize = BULLETICON_SIZE;

		bSize = true;
	}
	if (m_pos.y <= m_MainPos.y)
	{
		m_pos = m_MainPos;
	}

	if (bSize == true && bPos == true)
	{
		CPlayer* pPlayer = CGame::GetPlayer();
		pPlayer->GetCntSystem()->SetbMove(false);
	}


	// 弾アイコン
	m_pIcon->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 4.5f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pIcon->SetSize(m_fIconSize, m_fIconSize);

	// ×
	m_cross->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 2.0f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_cross->SetSize(m_fIconSize, m_fIconSize);

	// 桁数分カウンター
	for (int nCnt = 0; nCnt < MAX_BULLETCNT; nCnt++)
	{
		m_pCounter[nCnt]->GetNumber()->Set(D3DXVECTOR3(m_pos.x + (nCnt * m_fCntSize * 2.5f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pCounter[nCnt]->GetNumber()->SetSize(m_fCntSize, m_fCntSize);
	}
}

void CBulletCntMana::BecomeSub()
{
	const float AddPosY = 5.0f;
	const float SubSize = 1.0f;
	const float fDiv = 1.75f;

	static bool bSize = false;
	static bool bPos = false;

	m_pos.y += AddPosY;
	m_fIconSize -= SubSize;
	m_fCntSize -= SubSize;
	if (m_fCntSize <= BULLET_COUNT_SIZE / fDiv)
	{
		m_fCntSize = BULLET_COUNT_SIZE / fDiv;
		m_fIconSize = BULLETICON_SIZE / fDiv;

		bSize = true;
	}
	if (m_pos.y >= m_SubPos.y)
	{
		m_pos = m_SubPos;
	}

	if (bSize == true && bPos == true)
	{
		CPlayer* pPlayer = CGame::GetPlayer();
		pPlayer->GetCntSystem()->SetbMove(false);
	}

	// 弾アイコン
	m_pIcon->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 4.5f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pIcon->SetSize(m_fIconSize, m_fIconSize);

	// ×
	m_cross->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 2.0f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_cross->SetSize(m_fIconSize, m_fIconSize);

	// 桁数分カウンター
	for (int nCnt = 0; nCnt < MAX_BULLETCNT; nCnt++)
	{
		m_pCounter[nCnt]->GetNumber()->Set(D3DXVECTOR3(m_pos.x + (nCnt * m_fCntSize * 2.5f), m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pCounter[nCnt]->GetNumber()->SetSize(m_fCntSize, m_fCntSize);
	}
}

void CBulletCntMana::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	for (int nCnt = 0; nCnt < MAX_BULLETCNT; nCnt++)
	{
		m_pCounter[nCnt]->GetNumber()->Set(D3DXVECTOR3(m_pos.x + (nCnt * m_fCntSize * 2.5f), m_pos.y, 0.0f), m_rot);
	}
	m_cross->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 2.0f), m_pos.y, 0.0f), m_rot);
	m_pIcon->Set(D3DXVECTOR3(m_pos.x - (m_fCntSize * 4.5f), m_pos.y, 0.0f), m_rot);
}

//============
// 描画処理
//============
void CBulletCntMana::Draw(void)
{
}
//===================
// カウンター更新
//===================
void CBulletCntMana::UpdateCounter(void)
{	
	int aPosTexU[MAX_BULLETCNT] = {};	// 各桁の値格納用変数
	// 各桁の値計算用基準値変数
	int nData = BULLETCNTDATA * 10;
	int nData2 = BULLETCNTDATA;

	// 中身が０なら
	if (m_nCount <= 0)
	{
		m_nCount = 0;	// ０に設定
	}

	// テクスチャ切り替え
	for (int nCnt = 0; nCnt < MAX_BULLETCNT; nCnt++)
	{
		if (m_pCounter[nCnt] != nullptr)
		{// NULLチェック
			// 数字のポインタ取得
			CNumber* pNumber = m_pCounter[nCnt]->GetNumber();

			// 桁１つ分の値を計算
			aPosTexU[nCnt] = m_nCount % nData / nData2;

			// 次の桁用に基準値を1/10する
			nData /= 10;
			nData2 /= 10;

			// テクスチャ切り替え
			pNumber->SetTex((aPosTexU[nCnt] * 0.1f), (aPosTexU[nCnt] * 0.1f) + 0.1f, 0.0f, 1.0f);
		}
	}
}