//==============================
//
// 弾[bullet.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "item.h"
#include "renderer.h"
#include "manager.h"
#include "particle.h"
#include "debugproc.h"
#include "collision.h"
//==================
// コンストラクタ
//==================
CItem::CItem(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_type = TYPE_BULLET;

	m_bUse = true;
}
//================
// デストラクタ
//================
CItem::~CItem()
{

}
//===========
// 生成処理
//===========
CItem* CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CItem* pBullet = new CItem;
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
HRESULT CItem::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;
	m_rot = rot;
	m_move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	m_nLife = ITEM_LIFE;
	m_bUse = true;
	m_type = type;
	m_fFloatTime = 0.0f;


	//モデル生成
	const char* apFilename[TYPE_MAX] = 
	{ 
		"data\\MODEL\\carton00.x",
		"data\\MODEL\\battery00.x",
	
	};

	m_pModel = CModel::Create(apFilename[type], m_pos, m_rot);
	m_size = m_pModel->SetSize() * 2.0f;

	//オブジェクトの種類設定
	SetObjType(TYPE_ITEM);
	
	return S_OK;
}
//============
// 終了処理
//============
void CItem::Uninit(void)
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
void CItem::Update(void)
{
	if (m_bUse == true)
	{
		//移動
		Move();

		//寿命
		//m_nLife--;
		//if (m_nLife <= 0)
		//{
		//	m_bUse = false;
		//}
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
		return;
	}
}
//============
// 描画処理
//============
void CItem::Draw(void)
{
	if (m_bUse == true)
	{
		//デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//計算用マトリックス
		D3DXMATRIX mtxTrans;
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
void CItem::Move(void)
{
	const float fAdd = 0.05f;
	const float fHeight = 0.25f;

	// 浮遊時間を加算
	m_fFloatTime += fAdd;

	// 浮遊
	m_pos.y += sinf(m_fFloatTime) * fHeight;

	// 回転
	m_rot.y += fAdd;

	// 角度の正規化
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), m_rot, D3DCOLOR_RGBA(127, 255, 127, 255), 1, 5.0f, CParticle::TYPE_SMOKE);

	m_pModel->Set(m_pos, m_rot);
}
//============
// 効果
//============
void CItem::Bonus(void)
{
	CScoreMana* pTotalScore = CGame::GetTotalScore();

	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 pos = pPlayer->GetPos();
	switch (m_type)
	{
	case TYPE_BULLET:
	{
		int nContent = pPlayer->GetContents();
		if (nContent <= 0)
		{//中身０でランダム入手
			auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			size_t seed = static_cast<size_t>(now) ^ reinterpret_cast<size_t>(this);

			std::mt19937 mt((unsigned int)seed);
			std::uniform_int_distribution<int> dist(0, CBullet::TYPE_MAX - 1);

			int nType = dist(mt);

			pPlayer->SetBulletType((CBullet::TYPE)nType);
		}

		pPlayer->AddContents(ITEM_BULLET);

		CGame::GetFlash()->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f));

		pTotalScore->AddScore(100);
		break;
	}
	case TYPE_LIFE:
		pPlayer->Heal(ITEM_HEAL);
		CGame::GetFlash()->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f));

		pTotalScore->AddScore(200);

		break;
	default:
		break;
	}

	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_ITEM);


	m_bUse = false;
}
