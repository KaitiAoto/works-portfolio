//==============================
//
// 爆発[explosion.cpp]
// Author:kaiti
//
//==============================
#include "explosion.h"
#include "renderer.h"
#include "manager.h"
//静的メンバ変数
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;
//==================
// コンストラクタ
//==================
CExplosion::CExplosion(int nPriority) :
	m_fAngle(atan2f(EXPLOSION_SIZE, EXPLOSION_SIZE))
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fLength = 0;

	m_nCntAnim = 0;
	m_nPattermAnim = 0;
	m_type = TYPE_A;
	m_nLife = 0;

	m_bUse = true;
}
//================
// デストラクタ
//================
CExplosion::~CExplosion()
{
}
//===========
// 生成処理
//===========
CExplosion* CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CExplosion* pBullet;
	//
	pBullet = new CExplosion;
	//初期化
	pBullet->SetType(type);
	//初期化
	if (FAILED(pBullet->Init(pos, rot, EXPLOSION_SIZE, EXPLOSION_SIZE)))
	{
		delete pBullet;
		return nullptr;
	}

	//テクスチャ
	pBullet->BindTexture(m_pTexture);

	return pBullet;
}
//=====================
// テクスチャ読み込め
//=====================
HRESULT CExplosion::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\explosion000.png",
		&m_pTexture);

	return S_OK;
}
//=================
// テクスチャ破棄
//=================
void CExplosion::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	return S_OK;
}
//===============
// 初期化処理
//===============
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_move = D3DXVECTOR3(sinf(rot.z + D3DX_PI) * 2.0f, cosf(rot.z + D3DX_PI) * 2.0f, 0.0f);
	m_nLife = EXPLOSION_LIFE;

	m_fLength=(sqrtf(EXPLOSION_SIZE * EXPLOSION_SIZE + EXPLOSION_SIZE * EXPLOSION_SIZE) / 2.0f);
	
	//初期化
	CObject2D::Init(pos, rot, fWidth, fHeight);
	
	//テクスチャ設定
	SetTex(0.0f,0.125f,0.0f, 1.0f);

	if (m_type == TYPE_A)
	{
		SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	else if (m_type == TYPE_B)
	{
		SetColor(D3DCOLOR_RGBA(255, 0, 0, 255));
	}
	SetObjType(TYPE_EXPLOSION);

	return S_OK;
}
//============
// 終了処理
//============
void CExplosion::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CExplosion::Update(void)
{
	if (m_bUse == true)
	{
		//アニメーション
		Animation();

		//寿命
		if (m_nLife <= 0)
		{
			m_bUse = false;
		}
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
	}
}
//============
// 描画処理
//============
void CExplosion::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}
//======================
// アニメーション処理
//======================
void CExplosion::Animation(void)
{
	m_nCntAnim++;
	if ((m_nCntAnim % 10) == 0)
	{//一定時間経過
		//パターンNoを更新
		m_nPattermAnim = (m_nPattermAnim + 1) % 8;

		SetTex((m_nPattermAnim % 8) * 0.125f, (m_nPattermAnim % 8) * 0.125f + 0.125f, 0.0f, 1.0f);
	}
	if (m_nCntAnim == 64)
	{
		m_nLife = 0;
	}
}
