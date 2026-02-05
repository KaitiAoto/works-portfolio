//==============================
//
// 自販機[vender.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "vender.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "restock.h"
#include "gauge_enemybase.h"

//
CRestock* CVender::m_pRestock = nullptr;

//==================
// コンストラクタ
//==================
CVender::CVender(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_mtxWorld = {};

	m_pModel = NULL;

	m_bUse = true;
	m_bUseRestock = true;

	m_nRestock = 0;
	m_nCntReuse = 0;

	m_type = CBullet::TYPE_CAN;
	m_pCylinder = nullptr;
	m_pRestock = nullptr;

	m_pMapIcon = nullptr;
}
//================
// デストラクタ
//================
CVender::~CVender()
{

}
//===========
// 生成処理
//===========
CVender* CVender::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CBullet::TYPE type)
{
	CVender* pVender = new CVender;
	if (!pVender)
		return nullptr;

	if (FAILED(pVender->Init(pos, rot, type)))
	{
		delete pVender;
		return nullptr;
	}

	return pVender;
}
//===============
// 初期化処理
//===============
HRESULT CVender::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CBullet::TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_Offrot = m_rot;
	m_bUse = true;

	m_nRestock = MAX_RESTOCK;
	m_nCntReuse = NUM_REUSETIME;
	m_bUseRestock = true;
	m_type = type;
	m_bShake = false;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bRLShake = true;
	m_bCanRestock = false;

	//モデル生成
	const char* pFilename = SetModelName();

	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	m_size = m_pModel->SetSize();

	CShadow::Create(m_pos, m_rot, m_size.x * 1.25f, m_size.z * 1.25f);

	//オブジェクトの種類設定
	SetObjType(TYPE_VENDER);

	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		float radius = max(m_size.x, max(m_size.y, m_size.z)) * 0.5f;
		if (m_pRestock == nullptr)
		{
			m_pRestock = CRestock::Create("data\\TEXTURE\\restock02.png", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 1.35f - 100.0f, 0.0f), RESTOCK_SIZE, RESTOCK_SIZE / 2);
		}

		const char* pTexName[CEnemyBaseGauge::TYPE_MAX + 1] =
		{
			"data\\TEXTURE\\vendingIcon00.png",
			"data\\TEXTURE\\vendingIcon01.png",
			"data\\TEXTURE\\vendingIcon02.png",
			"data\\TEXTURE\\vendingIcon03.png",
		};

		CEnemyBaseGauge::TYPE Icontype = CEnemyBaseGauge::TYPE_DRINK;
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);

		if (m_type == CBullet::TYPE_CAN || m_type == CBullet::TYPE_PETBOTTLE || m_type == CBullet::TYPE_BOTTLE)
		{
			Icontype = CEnemyBaseGauge::TYPE_DRINK;
			col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.75f);
		}
		else if (m_type == CBullet::TYPE_ICE || m_type == CBullet::TYPE_SNACK)
		{
			Icontype = CEnemyBaseGauge::TYPE_FOOD;
			col = D3DXCOLOR(1.0f, 0.6f, 0.6f, 0.75f);
		}
		else if (m_type == CBullet::TYPE_CAPSULE || m_type == CBullet::TYPE_CIGARET || m_type == CBullet::TYPE_CARD)
		{
			Icontype = CEnemyBaseGauge::TYPE_GENERAL;
			col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 0.75f);
		}
		else
		{
			Icontype = CEnemyBaseGauge::TYPE_MAX;
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f);
		}

		m_pMapIcon = CMapEnemyBase::Create(pTexName[Icontype],m_pos, 15.0f, 20.0f);

		m_pCylinder = CMeshCylinder::Create("data\\TEXTURE\\gauge00.jpeg", D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), m_rot, radius / 4, 100, col, CMeshCylinder::TYPE_BOTHSIDES);
	}
	return S_OK;
}
//============
// 終了処理
//============
void CVender::Uninit(void)
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
void CVender::Update(void)
{
	if (m_bUse == true)
	{
		if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
		{
			if (m_bUseRestock == true)
			{
				if (m_pRestock != nullptr && m_bCanRestock == true)
				{
					if (m_bShake != true)
					{
						int nType;
						nType = rand() % SHAKE_MAX;

						m_ShakeType = (SHAKE)nType;
					}
					Shake();
					m_bShake = true;
					m_pRestock->SetUse(true);
				}
				else
				{
					m_pRestock->SetUse(false);
					m_rot = m_Offrot;
					m_pModel->SetRot(m_rot);
					m_bShake = false;
					m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				}
				m_pModel->SetScale(m_scale);
			}
			else if (m_bUseRestock == false)
			{
				m_rot = m_Offrot;
				m_pModel->SetRot(m_rot);
				m_bShake = false;
				m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
				m_pModel->SetScale(m_scale);

				m_nCntReuse--;
				if (m_nCntReuse <= 0)
				{
					m_nCntReuse = NUM_REUSETIME;
					m_bUseRestock = true;
					m_nRestock = MAX_RESTOCK;
				}
				m_pCylinder->SetUse(m_bUseRestock);
			}

			m_pMapIcon->SetUse(m_bUseRestock);
		}
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
	}
}
//===========
// 揺れる
//===========
void CVender::Shake(void)
{
	if (m_ShakeType == SHAKE_ROTY)
	{
		m_rot.y += 0.1f;
		// 角度の正規化
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2.0f;
		}
	}
	else if (m_ShakeType == SHAKE_ROTZ)
	{
		if (m_bRLShake == true)
		{
			m_rot.z += 0.05f;
			if (m_rot.z >= 0.5f)
			{
				m_bRLShake = false;
			}
		}
		else if (m_bRLShake == false)
		{
			m_rot.z -= 0.05f;
			if (m_rot.z <= -0.5f)
			{
				m_bRLShake = true;
			}
		}
	}
	else if (m_ShakeType == SHAKE_SCALEX)
	{
		m_scale.x += 0.01f;
		if (m_scale.x >= 1.5f)
		{
			m_scale.x = 1.0f;
		}
	}
	else if (m_ShakeType == SHAKE_SCALEY)
	{
		m_scale.y += 0.01f;
		if (m_scale.y >= 1.5f)
		{
			m_scale.y = 1.0f;
		}
	}
	
	m_pModel->SetRot(m_rot);
}

//============
// 描画処理
//============
void CVender::Draw(void)
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
//======================================
// 種類ごとのモデル名
//======================================
const char* CVender::SetModelName(void)
{
	const char* pFilename = "data\\MODEL\\vending_machine00.x";

	switch (m_type)
	{
	case CBullet::TYPE_CAN:
		pFilename = "data\\MODEL\\vending_machine00.x";
		break;
	case CBullet::TYPE_ICE:
		pFilename = "data\\MODEL\\vending_machine03.x";
		break;
	case CBullet::TYPE_CAPSULE:
		pFilename = "data\\MODEL\\vending_machine02.x";
		break;
	case CBullet::TYPE_PETBOTTLE:
		pFilename = "data\\MODEL\\vending_machine04.x";
		break;
	case CBullet::TYPE_DUST:
		pFilename = "data\\MODEL\\dustbox00.x";
		break;
	case CBullet::TYPE_SNACK:
		pFilename = "data\\MODEL\\vending_machine05.x";
		break;
	case CBullet::TYPE_CIGARET:
		pFilename = "data\\MODEL\\vending_machine01.x";
		break;
	case CBullet::TYPE_CARD:
		pFilename = "data\\MODEL\\vending_machine06.x";
		break;
	case CBullet::TYPE_BOTTLE:
		pFilename = "data\\MODEL\\vending_machine07.x";
		break;

	default:
		break;
	}

	return pFilename;
}
//=====================
// モデル名ごとの種類
//=====================
CBullet::TYPE CVender::SetType(const char* pFileName)
{
	CBullet::TYPE type = CBullet::TYPE_CAN;

	if (strcmp(pFileName, "data\\MODEL\\vending_machine00.x") == 0)
	{
		type = CBullet::TYPE_CAN;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine03.x") == 0)
	{
		type = CBullet::TYPE_ICE;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine02.x") == 0)
	{
		type = CBullet::TYPE_CAPSULE;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine04.x") == 0)
	{
		type = CBullet::TYPE_PETBOTTLE;
	}
	else if (strcmp(pFileName, "data\\MODEL\\dustbox00.x") == 0)
	{
		type = CBullet::TYPE_DUST;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine05.x") == 0)
	{
		type = CBullet::TYPE_SNACK;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine01.x") == 0)
	{
		type = CBullet::TYPE_CIGARET;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine06.x") == 0)
	{
		type = CBullet::TYPE_CARD;
	}
	else if (strcmp(pFileName, "data\\MODEL\\vending_machine07.x") == 0)
	{
		type = CBullet::TYPE_BOTTLE;
	}

	return type;
}
