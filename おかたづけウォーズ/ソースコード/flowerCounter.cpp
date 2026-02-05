//===============================================
//
// 花数カウンター[flowerCounter.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "flowerCounter.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "objectBillboard.h"
#include "screen_flash.h"

//===============================================
// コンストラクタ
//===============================================
CFlowerCnt::CFlowerCnt(int nPriority) :CObject(nPriority),
m_pCircle{},
m_pFlower{},
m_nCntFlower(0)
{
	
}
//===============================================
// デストラクタ
//===============================================
CFlowerCnt::~CFlowerCnt()
{
}
//===============================================
// 生成処理
//===============================================
CFlowerCnt* CFlowerCnt::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 生成
	CFlowerCnt* pFlowerCnt = new CFlowerCnt;
	// 初期化
	if (FAILED(pFlowerCnt->Init(pos, size)))
	{// NULLチェック
		delete pFlowerCnt;
		return nullptr;
	}
	return pFlowerCnt;	// ポインタを返す
}
//===============================================
// 初期化処理
//===============================================
HRESULT CFlowerCnt::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	for (int nCnt = 0; nCnt < CREATE_BOUQET; nCnt++)
	{
		if (m_pCircle[nCnt] == nullptr && m_pFlower[nCnt] == nullptr)
		{
			// モデル生成
			m_pCircle[nCnt] = CObjectX::Create("data\\MODEL\\flowerCnt00.x", pos, VECTOR3_NULL);
			m_pFlower[nCnt] = CObjectX::Create("data\\MODEL\\flower00.x", pos, VECTOR3_NULL);
			// 円のサイズ調整＆発光
			m_pCircle[nCnt]->SetRealSize(size);
			m_pCircle[nCnt]->SetEmissive(true);

			// 花のサイズ調整＆発光
			D3DXVECTOR3 flowersize = size * 0.75f;		
			m_pFlower[nCnt]->SetRealSize(flowersize);
			//m_pFlower[nCnt]->SetEmissive(true);
		}
		pos.x += size.x * 1.25f;	// 位置調整
	}

	ResetHaveFlower();

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CFlowerCnt::Uninit(void)
{
	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CFlowerCnt::Update(void)
{
}
//===============================================
// 描画処理
//===============================================
void CFlowerCnt::Draw(void)
{

}
//===============================================
// 花所持数加算
//===============================================
void CFlowerCnt::AddHaveFlower(void)
{
	if (m_pFlower[m_nCntFlower] != nullptr)
	{
		m_pFlower[m_nCntFlower]->SetEmissive(true);

	}
	m_nCntFlower++;

	if (m_nCntFlower >= CREATE_BOUQET)
	{
		CScreenFlash::Create("data\\TEXTURE\\bouquet02.png", D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(150.0f, 150.0f), true);

		// 花束を持っている状態にする
		CGame::GetPlayer()->SetBouqet(true);

		CSound* pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_GETBOUQUWT);
	}
}
//===============================================
// 花所持数リセット
//===============================================
void CFlowerCnt::ResetHaveFlower(void)
{
	for (int nCnt = 0; nCnt < CREATE_BOUQET; nCnt++)
	{
		if (m_pFlower[nCnt] != nullptr)
		{
			m_pFlower[nCnt]->SetEmissive(false);
		}
	}
	m_nCntFlower = 0;
}
//===============================================
//
//===============================================
void CFlowerCnt::SetShakeInfo(bool bShake, int nShakeTime, D3DXVECTOR3 MaxShake)
{
	for (int nCnt = 0; nCnt < CREATE_BOUQET; nCnt++)
	{
		if (m_pFlower[nCnt] != nullptr)
		{
			m_pFlower[nCnt]->SetShake(bShake, nShakeTime, MaxShake);
		}
		if (m_pCircle[nCnt] != nullptr)
		{
			m_pCircle[nCnt]->SetShake(bShake, nShakeTime, MaxShake);
		}
	}
}
