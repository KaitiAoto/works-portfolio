//==============================
//
// 弾[bullet.cpp]
// Author:kaiti
//
//==============================
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "objectBillboard.h"
#include "texture.h"
//==================
// コンストラクタ
//==================
CBullet::CBullet(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_type = TYPE_A;
	m_user = USER_PLAYER;

	m_bUse = true;

	m_nPriority = nPriority;
}
//================
// デストラクタ
//================
CBullet::~CBullet()
{

}
//===========
// 生成処理
//===========
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user)
{
	CBullet* pBullet = new CBullet;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pos, rot, user)))
	{
		delete pBullet;
		return nullptr;
	}

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user)
{
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * 1.5f, 0.0f, cosf(m_rot.y - D3DX_PI) * 1.5f);
	m_nLife = BULLET_LIFE;
	m_bUse = true;
	m_user = user;
	
	//モデル生成
	m_pModel = new CModel();
	m_pModel = CModel::Create("data\\MODEL\\can00.x", m_pos, m_rot);

	//オブジェクトの種類設定
	SetObjType(TYPE_BULLET);
	
	return S_OK;
}
//============
// 終了処理
//============
void CBullet::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}
	CObject::Release();
}
//============
// 更新処理
//============
void CBullet::Update(void)
{
	if (m_bUse == true)
	{
		//移動
		Move();
		//CEffect::Create(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), 60, 50);
		//CObjectBilldoard::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//寿命
		m_nLife--;
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
void CBullet::Draw(void)
{
	if (m_bUse == true)
	{
		//デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//モデル描画
		m_pModel->Draw();
	}
}
//===========
// 移動処理
//===========
void CBullet::Move(void)
{
	m_pos += m_move;
	m_pModel->Set(m_pos, m_rot);
}