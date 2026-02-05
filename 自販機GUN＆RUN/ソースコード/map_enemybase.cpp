//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "map_enemybase.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CMapEnemyBase::CMapEnemyBase(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nIdxTex = 0;
	m_bUse = true;
}
//================
// デストラクタ
//================
CMapEnemyBase::~CMapEnemyBase()
{
}
//===========
// 生成処理
//===========
CMapEnemyBase* CMapEnemyBase::Create(const char* pFileName,D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CMapEnemyBase* pObject2D;
	pObject2D = new CMapEnemyBase;
	//初期化
	pObject2D->Init(pFileName,pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CMapEnemyBase::Init(const char* pFileName,D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//値を代入
	m_pos = pos;
	m_bUse = true;
	m_nCntTime = 0;

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_BlinkCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	CObject2D::Init(pFileName, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight);

	// ミニマップの情報
	const float miniMapCenterX = CGame::GetMap()->GetPos().x;
	const float miniMapCenterY = CGame::GetMap()->GetPos().y;
	const float miniMapW = CGame::GetMap()->GetWidth();
	const float miniMapH = CGame::GetMap()->GetHeight();

	// ワールド座標を正規化
	const float worldSizeX = 2000.0f;
	const float worldSizeZ = 2000.0f;

	float normX = (pos.x + worldSizeX / 2.0f) / worldSizeX;
	float normZ = (pos.z + worldSizeZ / 2.0f) / worldSizeZ;

	// 中心を基準に描画
	float iconX = miniMapCenterX - miniMapW / 2.0f + normX * miniMapW;
	float iconY = miniMapCenterY - miniMapH / 2.0f + (1.0f - normZ) * miniMapH;

	m_pos = D3DXVECTOR3(iconX, iconY, 0.0f);

	CObject2D::SetPos(m_pos);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CMapEnemyBase::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CMapEnemyBase::Update(void)
{
	// 点滅する速さ
	float fSpeed = 0.25f;

	if (m_bBlink == true)
	{
		m_nCntTime++;

		if (m_bHelp == true)
		{
			m_BlinkCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			fSpeed = 0.25f;
		}
		else if (m_bHelp == false)
		{
			fSpeed = 0.05f;
			if (m_nCntTime >= 60)
			{
				m_nCntTime = 0;
				m_bBlink = false;
				return;
			}
		}

		float fBlink = 1.0f * sinf(fSpeed * m_nCntTime);
		
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
		m_col.r = col.r * (1.0f - fBlink) + m_BlinkCol.r * fBlink;
		m_col.g = col.g * (1.0f - fBlink) + m_BlinkCol.g * fBlink;
		m_col.b = col.b * (1.0f - fBlink) + m_BlinkCol.b * fBlink;
		m_col.a = col.a;

		// 色を設定
		CObject2D::SetColor(m_col);
		
	}
	else
	{
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 色を設定
		CObject2D::SetColor(m_col);
	}

	CObject2D::SetUse(m_bUse);
}
//============
// 描画処理
//============
void CMapEnemyBase::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}
