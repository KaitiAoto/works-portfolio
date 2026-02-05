//==============================
//
// 敵拠点[enemy_base.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "rival_shop.h"
#include "renderer.h"
#include "manager.h"
#include "score_manager.h"
#include "debugproc.h"
#include "particle.h"

// 静的メンバ変数

//==================
// コンストラクタ
//==================
CRivalShop::CRivalShop(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_mtxWorld = {};

	m_pModel = NULL;


	m_bUse = true;

	m_state = STATE_NONE;
}
//================
// デストラクタ
//================
CRivalShop::~CRivalShop()
{

}
//===========
// 生成処理
//===========
CRivalShop* CRivalShop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CRivalShop* pEnemyBase = new CRivalShop;
	if (!pEnemyBase)
		return nullptr;

	if (FAILED(pEnemyBase->Init(pos, rot)))
	{
		delete pEnemyBase;
		return nullptr;
	}

	return pEnemyBase;
}
//===============
// 初期化処理
//===============
HRESULT CRivalShop::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	m_bUse = true;
	//モデル生成
	m_pModel = CModel::Create("data\\MODEL\\convenience_store02.x", m_pos, m_rot);
	m_size = m_pModel->SetSize();
	//オブジェクトの種類設定
	SetObjType(TYPE_STAGE);

	CShadow::Create(m_pos, m_rot, m_size.x * 0.75f, m_size.z * 0.75f);

	return S_OK;
}
//============
// 終了処理
//============
void CRivalShop::Uninit(void)
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
void CRivalShop::Update(void)
{
	// モードがゲームなら
	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		// チュートリアル以外
		if (CGame::GetMode() != CGame::MODE_TUTORIAL)
		{
			// 使用していれば
			if (m_bUse == true)
			{
				SetState();
			}
			else if (m_bUse == false)
			{//使っていないなら
				Uninit();
			}
		}
	}
}
//============
// 描画処理
//============
void CRivalShop::Draw(void)
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
	
	if (m_bUse == true)
	{
		//モデル描画
		m_pModel->Draw();
	}
}
//
// スケール変更
//
void CRivalShop::Scale(void)
{
	const float fSubScale = 0.01f;
	D3DXVECTOR3 scale;

	switch (m_state)
	{
	case STATE_NONE:
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		break;
	case STATE_THRIVING:
		scale = m_pModel->GetScale();
		scale.x += fSubScale;
		scale.y += fSubScale;
		scale.z += fSubScale;

		if (scale.x >= 1.2f)
		{
			CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2.0f), m_pos.z), m_rot, D3DCOLOR_RGBA(255, 255, 1, 255), 1, 15.0f, CParticle::TYPE_NONE);
			scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
		break;
	case STATE_STUGGLING:
		scale = m_pModel->GetScale();
		scale.x -= fSubScale / 2;
		scale.y -= fSubScale / 2;
		scale.z -= fSubScale / 2;

		if (scale.x <= 0.8f)
		{
			CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2.0f), m_pos.z), m_rot, D3DCOLOR_RGBA(1, 1, 255, 255), 1, 10.0f, CParticle::TYPE_NONE);
			scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}

		break;
	default:
		break;
	}

	m_pModel->SetScale(scale);
}
void CRivalShop::SetState(void)
{
	int nCntNone = 0;
	int nCntThriving = 0;
	int nCntStuggling = 0;

	CObject* pObj = CObject::GetTop(PRIORITY_ENEMYBASE);
	while (pObj != NULL)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();//タイプ取得
		switch (type)
		{
		case CObject::TYPE_ENEMY_BASE:		
		{
			CEnemyBase* pEnemyBase = (CEnemyBase*)pObj;

			CEnemyBase::STATE state = pEnemyBase->GetState();

			switch (state)
			{
			case STATE_NONE:
				nCntNone++;
				break;
			case STATE_THRIVING:
				nCntThriving++;
				break;
			case STATE_STUGGLING:
				nCntStuggling++;
				break;
			default:
				break;
			}

			break;
		}
		default:
			break;
		}
		pObj = pObjNext;
	}

	
	if (nCntThriving >= nCntNone && nCntThriving >= nCntStuggling)
	{
		m_state = STATE_STUGGLING;
	}
	else if (nCntStuggling >= nCntNone && nCntStuggling >= nCntThriving)
	{
		m_state = STATE_THRIVING;
	}
	else
	{
		m_state = STATE_NONE;
	}

	Scale();
}