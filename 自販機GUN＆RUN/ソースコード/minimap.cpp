//==============================
//
// ’e[bullet.h]
// Author : Kaiti Aoto
//
//==============================
#include "minimap.h"
#include "game.h"

CMapPlayer* CMap::m_pMapPlayer = nullptr;

CMap::CMap()
{
}

CMap::~CMap()
{
}

CMap* CMap::Create(const D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CMap* pMap = new CMap();

	pMap->Init(pos, fWidth, fHeight);

	return pMap;
}

HRESULT CMap::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	m_pos = pos;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	CObject2D::Create("data\\TEXTURE\\map01.png", pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 8);

	m_pMapPlayer = CMapPlayer::Create(pos, 15.0f, 20.0f);
	m_pMapPlayer->SetUse(false);

	return S_OK;
}

void CMap::Uninit(void)
{
	CObject::Release();
}

void CMap::Update(void)
{

}

void CMap::Draw(void)
{

}

