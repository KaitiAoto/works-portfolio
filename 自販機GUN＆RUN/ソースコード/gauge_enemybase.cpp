//==============================
//
//  タイマー処理[.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge_enemybase.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
#include "object2D.h"
#include "map_enemybase.h"

//==================
// コンストラクタ
//==================
CEnemyBaseGauge::CEnemyBaseGauge(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
	m_pGauge = nullptr;
}
//================
// デストラクタ
//================
CEnemyBaseGauge::~CEnemyBaseGauge()
{
}
//===========
// 生成処理
//===========
CEnemyBaseGauge* CEnemyBaseGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col, TYPE type, CEnemyBase* pEnemyBase)
{
	CEnemyBaseGauge* pGause;
	//
	pGause = new CEnemyBaseGauge;
	//初期化
	if (FAILED(pGause->Init(pos, base, fHeight, col,type)))
	{
		delete pGause;
		return nullptr;
	}
	pGause->m_pEnemyBase = pEnemyBase;
	return pGause;
}
//=============
// 初期化処理
//=============
HRESULT CEnemyBaseGauge::Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col, TYPE type)
{
	m_pos = pos;
	m_Base = base;
	m_MaxBase = base;
	m_fHeight = fHeight;
	m_col = col;
	m_pGauge = CGaugeBillboard::Create(m_pos, m_Base, m_fHeight, m_col);
	m_type = type;

	const char* pTexName = {};	// テクスチャ名格納用変数
	
	// 種類別にテクスチャ名を指定
	switch (type)
	{
	case TYPE_DRINK:
		pTexName = "data\\TEXTURE\\stock00.png";
		m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f,1.0f);
		break;
	case TYPE_FOOD:
		pTexName = "data\\TEXTURE\\stock01.png";
		m_col = D3DXCOLOR(1.0f, 0.6f, 0.6f, 1.0f);
		break;
	case TYPE_GENERAL:
		pTexName = "data\\TEXTURE\\stock02.png";
		m_col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
		break;
	default:
		break;
	}


	m_pFrame = CObjectBillboard::Create("data\\TEXTURE\\gauge_frame00.png", m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), (GAUGE_X * m_Base) / 2, m_fHeight, 7);

	m_pFull = CObjectBillboard::Create("data\\TEXTURE\\full00.png", D3DXVECTOR3(m_pos.x + ((GAUGE_X * m_Base) / 2) + m_fHeight, m_pos.y, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fHeight * 2.0f, m_fHeight, 7);

	m_pIcon = CObjectBillboard::Create(pTexName, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_fHeight * 3.0f, m_fHeight, 7);

	//オブジェクトの種類設定
	SetObjType(TYPE_GAUGE);

	return S_OK;
}
//==========
// 終了処理
//==========
void CEnemyBaseGauge::Uninit(void)
{
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		delete m_pGauge;
		m_pGauge = nullptr;
	}
	CObject::Release();
}
//==========
// 更新処理
//==========
void CEnemyBaseGauge::Update(void)
{
	if (m_bDraw == true)
	{
		m_pGauge->SetBase(m_Base);
		m_pGauge->SetRate(m_rate);
		m_pGauge->SetColor(m_col);
		m_pGauge->SetPos(m_pos);
		m_pGauge->Set();

		D3DXMATRIX matView, matInv;
		//デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		pDevice->GetTransform(D3DTS_VIEW, &matView);

		matView._41 = matView._42 = matView._43 = 0.0f;
		D3DXMatrixInverse(&matInv, NULL, &matView);

		D3DXVECTOR3 vRight(matInv._11, matInv._12, matInv._13);

		D3DXVECTOR3 vCenter = m_pos;
		D3DXVECTOR3 vPos = vCenter + vRight * (m_fHeight * 8);

		m_pFull->SetPos(vPos);
		m_pFull->SetDraw(m_bFull);
	}
	else
	{
		m_pFull->SetDraw(false);
	}

	m_pIcon->SetDraw(m_bDraw);
	m_pFrame->SetDraw(m_bDraw);
}
//===========
// 描画処理
//===========
void CEnemyBaseGauge::Draw(void)
{
	if (m_bDraw == true)
	{
		m_pGauge->Draw();
	}
}