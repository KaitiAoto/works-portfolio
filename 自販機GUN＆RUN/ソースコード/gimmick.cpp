//==============================
//
// 敵[enemy.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "gimmick.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "collision.h"
#include "enemy.h"
//==================
// コンストラクタ
//==================
CGimmick::CGimmick(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;


	m_type = TYPE_CAR;

	m_bUse = true;

	m_fMoveDis = 0.0f;
	m_fMaxDis = MAX_DIS;
}
//================
// デストラクタ
//================
CGimmick::~CGimmick()
{

}
//===========
// 生成処理
//===========
CGimmick* CGimmick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CGimmick* pBullet = new CGimmick;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pos, rot, type)))
	{
		delete pBullet;
		return nullptr;
	}

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CGimmick::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * GIMMICK_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * GIMMICK_SPEED);

	m_bUse = true;
	m_type = type;
	m_posOffSet = pos;
	if (type == TYPE_CAR)
	{
		m_pModel = CModel::Create("data\\MODEL\\car000.x", m_pos, m_rot);
	}

	m_size = m_pModel->SetSize();

	//オブジェクトの種類設定
	SetObjType(TYPE_GIMMICK);

	return S_OK;
}
//============
// 終了処理
//============
void CGimmick::Uninit(void)
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
void CGimmick::Update(void)
{
	if (m_bUse == true)
	{
		//移動
		Move();
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
	}
}
//============
// 描画処理
//============
void CGimmick::Draw(void)
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
void CGimmick::Move(void)
{
	//if (CGame::GetMode() != CGame::MODE_TUTORIAL)
	{
		CEffect::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z), m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 0, 7);

		m_posOld = m_pos;

		m_pos += m_move;

		m_fMoveDis += D3DXVec3Length(&m_move);

		if (m_fMoveDis >= m_fMaxDis)
		{
			m_rot.y += D3DX_PI;
			if (m_rot.y > D3DX_PI * 2.0f)
			{
				m_rot.y -= D3DX_PI * 2.0f;
			}

			m_fMoveDis = 0.0f;
			m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * GIMMICK_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * GIMMICK_SPEED);
		}

		m_pModel->Set(m_pos, m_rot);
	}
}
//=====================
// モデル名ごとの種類
//=====================
CGimmick::TYPE CGimmick::SetType(const char* pFileName)
{
	CGimmick::TYPE type = CGimmick::TYPE_CAR;

	if (strcmp(pFileName, "data\\MODEL\\bluecar.x") == 0)
	{
		type = CGimmick::TYPE_CAR;
	}
	//else if (strcmp(pFileName, "data\\MODEL\\vending_machine03.x") == 0)
	//{
	//}
	return type;
}
