//==============================
//
// 敵拠点[enemy_base.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "enemy_base.h"
#include "renderer.h"
#include "manager.h"
#include "score_manager.h"
#include "debugproc.h"
#include "particle.h"

// 静的メンバ変数
int CEnemyBase::m_nNum = 0;

//==================
// コンストラクタ
//==================
CEnemyBase::CEnemyBase(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_bUse = true;

	m_bBlinkIcon = false;

	m_state = STATE_NONE;

}
//================
// デストラクタ
//================
CEnemyBase::~CEnemyBase()
{

}
//===========
// 生成処理
//===========
CEnemyBase* CEnemyBase::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemyBase* pEnemyBase = new CEnemyBase;
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
HRESULT CEnemyBase::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	m_nLife = ENEMY_BASE_LIFE;
	m_bUse = true;
	//モデル生成
	m_pModel = CModel::Create("data\\MODEL\\convenience_store00.x", m_pos, m_rot);
	m_size = m_pModel->SetSize();
	m_pBreakModel = CModel::Create("data\\MODEL\\convenience_store01.x", m_pos, m_rot);
	//オブジェクトの種類設定
	SetObjType(TYPE_ENEMY_BASE);

	CShadow::Create(m_pos, m_rot, m_size.x * 0.75f, m_size.z * 0.75f);

	// モードがゲームなら
	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
		{
			float GauseY = m_pos.y + (m_size.y);
			D3DXVECTOR3 Pos = { m_pos.x, m_pos.y + GauseY - (nCnt * 20.0f), m_pos.z };

			m_nStock[nCnt] = MAX_STOCK;
			float GeuseBase = (float)m_nStock[nCnt] / 1.5f;
			m_pGauge[nCnt] = CEnemyBaseGauge::Create(Pos, GeuseBase, 10.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), (CEnemyBaseGauge::TYPE)nCnt, this);
		}

		m_pMapIcon = CMapEnemyBase::Create("data\\TEXTURE\\conveniIcon00.jpg", m_pos, 25.0f, 25.0f);
	
		CScoreMana* pBreakScore = CGame::GetBreakCnt();
		pBreakScore->AddScore(1);

		m_Help = CObject2D::Create("data\\TEXTURE\\help00.png", D3DXVECTOR3(CGame::GetMap()->GetPos().x, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 250.0f, 120.0f, 8);
		m_Help->SetUse(false);

		// 総数を増やす
		m_nNum++;
	}


	return S_OK;
}
//============
// 終了処理
//============
void CEnemyBase::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}
	if (m_pBreakModel != NULL)
	{
		m_pBreakModel->Uninit();
		delete m_pBreakModel;
		m_pBreakModel = NULL;
	}
	
	m_nNum = 0;

	CObject::Release();
}
//============
// 更新処理
//============
void CEnemyBase::Update(void)
{
	// モードがゲームなら
	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		// チュートリアル以外
		if (CGame::GetMode() != CGame::MODE_TUTORIAL)
		{
			CScoreMana* pBreakScore = CGame::GetBreakCnt();

			// 使用していれば
			if (m_bUse == true)
			{
				// ゲージ
				for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
				{
					float rate = (float)m_nStock[nCnt] / (float)MAX_STOCK;
					rate = max(0.0f, min(rate, 1.0f));

					m_pGauge[nCnt]->SetRate(rate);
					m_pGauge[nCnt]->SetDraw(m_bRespawn);

					if (m_nStock[nCnt] >= MAX_STOCK)
					{
						m_pGauge[nCnt]->SetFullUse(true);
					}
					else
					{
						m_pGauge[nCnt]->SetFullUse(false);
					}
				}

				// アイコンの点滅確認
				BlinkIcon();
				SetState();

				// ライフが０なら
				if (m_nLife <= 0)
				{
					CManager::GetCamera()->SetShake(20.0f, 20.0f, 60);

					CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 1.5f), m_pos.z), m_rot, D3DCOLOR_RGBA(255, 1, 1, 255), 30, 8.0f, CParticle::TYPE_NONE);

					m_bUse = false;

					CSound* pSound = CManager::GetSound();
					pSound->PlaySound(CSound::SOUND_LABEL_BREAK);

					//パッド
					CInputPad* pInputPad = CManager::GetInputPad();
					pInputPad->SetVibration(30000, 30000, 0, 60);


					pBreakScore->AddScore(-1);

					for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
					{
						m_pGauge[nCnt]->SetDraw(false);
					}

					m_Help->SetUse(false);

					if (CGame::GetMode() != CGame::MODE_FIN)
					{
						CScreenFlash::Create(nullptr, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

					m_nNum--;
					if (m_nNum <= 0)
					{
						if (CGame::GetMode() != CGame::MODE_FIN)
						{
							CGame::SetMode(CGame::MODE_FIN);
							return;
						}
					}
				}

				if (CGame::GetPlayer()->GetUse() == false)
				{
					m_nLife = 0;
				}
			}
			else if (m_bUse == false)
			{//使っていないなら
				//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), m_rot, D3DCOLOR_RGBA(127, 127, 127, 255), 1, 15.0f, CParticle::TYPE_SMOKE);
			}
		}
		if (m_pMapIcon != nullptr)
		{
			m_pMapIcon->SetUse(m_bUse);
		}
	}
}
//============
// 描画処理
//============
void CEnemyBase::Draw(void)
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
	else
	{
		// 破壊モデル描画
		m_pBreakModel->Draw();
	}
}
//================
// ダメージ処理
//================
void CEnemyBase::Hit(const CBullet::TYPE type)
{
	for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
	{
		CEnemyBaseGauge::TYPE HitType = SearchHitType(type);

		if (HitType == m_pGauge[nCnt]->GetType())
		{
			CSound* pSound = CManager::GetSound();

			CScoreMana* pTotalScore = CGame::GetTotalScore();

			if (m_nStock[nCnt] < MAX_STOCK)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_ADD);

				m_nStock[nCnt]++;
				pTotalScore->AddScore(100);

				if (m_nStock[nCnt] == MAX_STOCK)
				{
					pTotalScore->AddScore(500);
					pSound->PlaySound(CSound::SOUND_LABEL_CONVENI);
				}
			}
			if (m_nStock[nCnt] > MAX_STOCK)
			{
				m_nStock[nCnt] = MAX_STOCK;
			}	
			break;
		}
	}
}
//=====================
// 在庫を減らす処理
//=====================
void CEnemyBase::SoldOut(void)
{
	int nType = 0;
	nType = rand() % CEnemyBaseGauge::TYPE_MAX;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	switch ((CEnemyBaseGauge::TYPE)nType)
	{
	case CEnemyBaseGauge::TYPE_DRINK:
		col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case CEnemyBaseGauge::TYPE_FOOD:
		col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		break;
	case CEnemyBaseGauge::TYPE_GENERAL:
		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
	m_pMapIcon->SetBlink(col, true);

	m_nStock[nType]--;

	if (m_nStock[nType] <= 0)
	{
		m_nStock[nType] = 0;
		m_nLife = 0;
	}
}
//========================
// アイコンが点滅するか
//========================
void CEnemyBase::BlinkIcon(void)
{
	for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
	{
		if (m_nStock[nCnt] < MAX_STOCK / 3)
		{
			m_pMapIcon->SetBlink(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);
			m_pMapIcon->SetHelp(true);
			m_Help->SetUse(true);
			break;
		}
		else
		{
			m_pMapIcon->SetHelp(false);
			m_Help->SetUse(false);
		}
	}
}
//
// スケール変更
//
void CEnemyBase::Scale(void)
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
void CEnemyBase::SetState(void)
{
	int totalStock = 0;
	bool bSTUGGLING = false;

	for (int nCnt = 0; nCnt < STOCK_TYPE; nCnt++)
	{
		if (m_nStock[nCnt] < MAX_STOCK / 3)
		{
			bSTUGGLING = true;
			break;
		}
		totalStock += m_nStock[nCnt];
	}

	if (bSTUGGLING == false)
	{
		// 繁盛していない
		if (totalStock < (MAX_STOCK * STOCK_TYPE) / 3)
		{
			m_state = STATE_STUGGLING;
		}
		// 繁盛している
		else if (totalStock >= STOCK_TYPE * 8)
		{
			m_state = STATE_THRIVING;
		}
		// 通常
		else
		{
			m_state = STATE_NONE;
		}
	}
	else
	{
		m_state = STATE_STUGGLING;
	}

	Scale();
}
//=======================
// 何の弾と当たったか
//=======================
CEnemyBaseGauge::TYPE CEnemyBase::SearchHitType(CBullet::TYPE type)
{
	CEnemyBaseGauge::TYPE HitType = CEnemyBaseGauge::TYPE_DRINK;

	// どの弾がどの種類かを判別
	switch (type)
	{
		case CBullet::TYPE_CAN:
			HitType = CEnemyBaseGauge::TYPE_DRINK;
			break;
		case CBullet::TYPE_CAPSULE:
			HitType = CEnemyBaseGauge::TYPE_GENERAL;
			break;
		case CBullet::TYPE_ICE:
			HitType = CEnemyBaseGauge::TYPE_FOOD;
			break;
		case CBullet::TYPE_PETBOTTLE:
			HitType = CEnemyBaseGauge::TYPE_DRINK;
			break;
		case CBullet::TYPE_DUST:
			HitType = CEnemyBaseGauge::TYPE_MAX;
			break;
		case CBullet::TYPE_SNACK:
			HitType = CEnemyBaseGauge::TYPE_FOOD;
			break;
		case CBullet::TYPE_CIGARET:
			HitType = CEnemyBaseGauge::TYPE_GENERAL;
			break;
		case CBullet::TYPE_CARD:
			HitType = CEnemyBaseGauge::TYPE_GENERAL;
			break;
		case CBullet::TYPE_BOTTLE:
			HitType = CEnemyBaseGauge::TYPE_DRINK;
			break;
		default:
		break;
	}

	return HitType;	// 種類を返す
}

