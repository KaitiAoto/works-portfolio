//===============================================
//
// インタラクトオブジェクト処理[interact.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "interact.h"
#include "manager.h"
#include "debugproc.h"
#include "particle.h"

// 静的メンバ変数
int CInteract::m_nNumDust = 0;
int CInteract::m_nNumGoods = 0;
int CInteract::m_nNumFlower = 0;

//===============================================
// コンストラクタ
//===============================================
CInteract::CInteract(int nPriority) : CObjectPhysics(nPriority)
{
	// 値をクリア
	m_pCircle = nullptr;
	m_size = VECTOR3_NULL;
	m_AreaPos = VECTOR3_NULL;
	m_type = TYPE_DUST;
	m_fRadius = 0.0f;

	m_bRising = false;			// 上昇しない
	m_bAsh = false;				// 灰ではない
	m_bGather = false;			// 収集されていない
	m_bDrop = false;			// 落ちるか
	m_bRLShake = false;			// どっちに揺れるか
}
//===============================================
// デストラクタ
//===============================================
CInteract::~CInteract()
{
}
//===============================================
// 種類ごとのランダム生成処理
//===============================================
CInteract* CInteract::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	switch (type)
	{
	case TYPE_DUST:
		return CInteractDust::Create(pos, rot, type);
		break;
	case TYPE_GOODS:
		return CInteractGoods::Create(pos, rot, type);
		break;
	case TYPE_FLOWER:
		return CInteractFlower::Create(pos, rot, type);
		break;
	default:
		return nullptr;
		break;
	}
}
//===============================================
// ランダムじゃない生成処理
//===============================================
CInteract* CInteract::Create(const char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight, TYPE type)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;

	pInteract->Init(Filename, pos, rot, fWeight, type);

	return pInteract;
}
//===============================================
// 種類ごとに複数個ランダム追加処理
//===============================================
void CInteract::RandAdd(TYPE type, D3DXVECTOR3 pos, int nNumCreate, float fSpace)
{
	// posを中心にfSpace分間をあけてnNumCreate分,四角になるようにクリエイト

	// インタラクトオブジェクトランダム大量配置
	const int gridCountX = nNumCreate / 2;		// X方向の数
	const int gridCountZ = nNumCreate / 2;		// Z方向の数

	for (int x = -gridCountX / 2; x < gridCountX / 2; ++x)
	{
		for (int z = -gridCountZ / 2; z < gridCountZ / 2; ++z)
		{
			D3DXVECTOR3 Pos(x * fSpace,0.0f,z * fSpace);
			pos += Pos;

			CInteract::Create(pos, D3DXVECTOR3(0, 0, 0), type);
		}
	}
}
//===============================================
// 初期化処理
//===============================================
HRESULT CInteract::Init(const char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight, TYPE type)
{	
	m_type = type;

	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 円の色	

	// ゴミの場合
	if (m_type == TYPE_DUST)
	{
		m_nNumDust++;
		col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 0.5f);
	}
	// 生活雑貨の場合
	else if (m_type == TYPE_GOODS)
	{
		m_nNumGoods++;
		col = D3DXCOLOR(0.5f, 1.0f, 0.0f, 1.0f);
	}
	// 生活雑貨の場合
	else if (m_type == TYPE_FLOWER)
	{
		m_nNumFlower++;
		col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
	}
	col.a = 0.25f;
	// 親クラスの初期化
	CObjectPhysics::Init(Filename, pos, rot, fWeight);

	m_size = GetSize();			// サイズ取得
	pos.y = 0.1f;				// 位置を円用に少し上げる
	m_fRadius = max(m_size.x, max(m_size.y, m_size.z)) * 0.75f;							// 半径計算
	//m_pCircle = CMeshCircle::Create(pos, rot, m_fRadius, col, CMeshCircle::TYPE_OUT);	// 円生成
	//m_pCircle->SetUse(true);	// 円を使用しない状態に

	SetObjType(TYPE_INTERACT);	// オブジェクトのタイプ
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CInteract::Uninit(void)
{
	bool bStageOut = false;

	// 種類がごみ
	if (m_type == TYPE_DUST)
	{
		m_nNumDust--;	// ごみの最大数を減らす

		// ステージ下に行っていたら
		if (GetPos().y < 0.0f && m_bAsh == false)
		{
			bStageOut = true;
			CGame::GetBoss()->Hit(2);

			// パーティクル生成
			CParticle::Create(GetPos(), VECTOR3_NULL, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 15, 8, CParticle::TYPE_NONE);
		}
		// 上昇したなら
		else if(m_bRising == true)
		{
			// ミスとしてカウント
			CGame::GetBoss()->GetAI()->AddMiss(TYPE_DUST);
		}
	}
	// 種類が生活雑貨
	else if (m_type == TYPE_GOODS)
	{
		m_nNumGoods--;	// 生活雑貨の最大数を減らす

		// ステージ下に行っていたら
		if (GetPos().y < 0.0f)
		{
			bStageOut = true;

			// ミスとしてカウント
			CGame::GetBoss()->GetAI()->AddMiss(TYPE_GOODS);

			if (CGame::GetTutorial() != nullptr && CGame::GetType() == CGame::TYPE_TUTORIAL)
			{
				if (CGame::GetTutorial()->GetMode() == CTutorial::MODE_MISS)
				{
					CGame::GetTutorial()->SetClear(true);
				}
			}
		}
	}
	// 種類が花
	else if (m_type == TYPE_FLOWER)
	{
		m_nNumFlower--;	// 生活雑貨の最大数を減らす

		// ステージ下に行っていたら
		if (GetPos().y < 0.0f)
		{
			bStageOut = true;

			CGame::GetBoss()->Heal(1);
			// ミスとしてカウント
			CGame::GetBoss()->GetAI()->AddMiss(TYPE_GOODS);
		}
	}

	// サークルがあれば
	if (m_pCircle != nullptr)
	{
		// サークルの終了処理
		m_pCircle->Uninit();
		m_pCircle = nullptr;
	}

	// ステージ外なら
	if (bStageOut == true && (CGame::GetMode() == CGame::MODE_PLAY || CGame::GetMode() == CGame::MODE_NONE))
	{
		// ゴミ箱を揺らす
		ShakeDustBox();
	}

	// 親クラスの破棄
	CObjectPhysics::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CInteract::Update(void)
{
	// 上昇するなら
	if (m_bRising == false)
	{
		// 自身が花でプレイヤーが花束を持っていない
		if (m_type == TYPE_FLOWER && CGame::GetPlayer()->GetHaveBouquet() != true)
		{
			// プレイヤーとの判定
			if (CollRadius() == true)
			{
				// 位置調整
				D3DXVECTOR3 pos = GetPos();
				pos.y += 50.0f;

				// パーティクル生成
				CParticle::Create(pos, VECTOR3_NULL, D3DXCOLOR(1.0f, 0.25f, 0.75f, 1.0f), 15, 8, CParticle::TYPE_NONE);

				CGame::GetPlayer()->GetFlowerCnt()->AddHaveFlower();	// 花数追加
				CGame::GetPlayer()->Heal(3);							// プレイヤーを回復
				CGame::GetBoss()->Hit(1);								// ボスにダメージ

				CSound* pSound = CManager::GetSound();
				pSound->PlaySound(CSound::SOUND_LABEL_GETFLOWER);

				// 終了処理
				Uninit();
			}
		}

		// NULLチェック
		if (m_pCircle != nullptr)
		{
			// 円の位置設定
			m_pCircle->SetPos(GetPos());
		}
	}
	// 上昇するなら
	else if(m_bRising == true)
	{
		// 上昇移動
		RisingMove();

		// NULLチェック
		if (m_pCircle != nullptr)
		{
			m_pCircle->SetUse(false);	// 円を描画しない
		}
	}
	// 親クラスの更新
	CObjectPhysics::Update();

	if (m_type == TYPE_FLOWER)
	{
		Reaction();
	}
}
//===============================================
// 描画処理
//===============================================
void CInteract::Draw(void)
{
	CObjectPhysics::Draw();
}
//===============================================
// 半径でのプレイヤーとの判定
//===============================================
bool CInteract::CollRadius(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerSize = pPlayer->GetSize();

	// 半径をサイズから算出
	float fPlayerRadius = max(PlayerSize.x, max(PlayerSize.y, PlayerSize.z)) * 0.5f;

	// 半径での判定
	if (CManager::GetColl()->Radius(PlayerPos, fPlayerRadius, GetPos(), m_fRadius) == true)
	{// 範囲内なら
		return true;
	}
	
	return false;
}
//===============================================
// リアクション
//===============================================
void CInteract::Reaction(void)
{
	D3DXVECTOR3 rot = GetRot();

	if (m_bRLShake == true)
	{
		rot.z += 0.025f;
		if (rot.z >= 0.5f)
		{
			m_bRLShake = false;
		}
	}
	else if (m_bRLShake == false)
	{
		rot.z -= 0.025f;
		if (rot.z <= -0.5f)
		{
			m_bRLShake = true;
		}
	}

	SetRot(rot);
}
//===============================================
// 上昇移動
//===============================================
void CInteract::RisingMove(void)
{
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, RISING_SPEED, 0.0f);

	if (m_bDrop == true && GetPos().y > 0.0f)
	{
		m_bRising = false;

		// ベクトル算出
		D3DXVECTOR3 vec = GetPos() - m_AreaPos;
		D3DXVec3Normalize(&vec, &vec);

		// 距離を取得
		D3DXVECTOR3 diff = GetPos() - m_AreaPos;
		float fPower = 0.25f;	// 最大吹き飛ばし力
		float fDist = D3DXVec3Length(&diff);
		float fMaxRadius = m_fRadius;

		// 範囲に応じて威力減少
		float fRate = 1.0f - (fDist / fMaxRadius);
		// 吹き飛ばしベクトルを算出
		move =  -(vec * (fPower * fRate));
		move.y = 0.0f;

		SetBlow(false, move);
	}
	else
	{
		// 上昇移動
		SetMove(move);
	}
	// 最大まで上昇したら
	if (GetPos().y >= MAX_RISING)
	{
		// 終了
		Uninit();

		if (CGame::GetType() == CGame::TYPE_TUTORIAL)
		{
			if (CGame::GetTutorial() != nullptr && CGame::GetTutorial()->GetMode() == CTutorial::MODE_GOODS)
			{
				CGame::GetTutorial()->SetClear(true);
			}
		}

		return;
	}
}
//===============================================
// 灰になる処理
//===============================================
void CInteract::ChangeAsh(void)
{
	// 今の情報を保存
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CInteract* pOld = this;

	// 新しいオブジェクト(灰)を生成
	CInteract* pNew = CInteract::Create(AshFilename, pos, rot, 1, TYPE_DUST);
	pNew->m_bAsh = true;	// 灰化していることにする

	// 前回のオブジェクトを削除
	pOld->Uninit();
}
//===============================================
// 上昇するか設定
//===============================================
void CInteract::SetRising(void)
{
	int nCntRising = 0;	// 上昇数をカウント

	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();

		// 種類が一致していれば
		if (type == CObject::TYPE_INTERACT)
		{
			// 物理オブジェクト情報取得
			CInteract* pObjInteract = (CInteract*)pObj;

			// 収集エリアされていれば
			if (pObjInteract->GetGather() == true)
			{
				// 上昇させる
				pObjInteract->m_bRising = true;

				nCntRising++;	// 上昇数加算
			}
			// されていなければ
			else
			{
				// 上昇させない
				pObjInteract->m_bRising = false;
			}
		}
		pObj = pObjNext;
	}

	// 上昇数の半分をダメージ
	CGame::GetBoss()->Hit((int)(nCntRising / 5.0f));
}
//===============================================
// ゴミ箱を揺らす
//===============================================
void CInteract::ShakeDustBox(void)
{
	if (CGame::GetField() != nullptr)
	{
		// ステージモデルのみ調べる
		CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageUI));
		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();
			CObject::TYPE type = pObj->GetObjType();

			// 種類が一致していれば
			if (type == CObject::TYPE_STAGE)
			{
				// ステージモデル情報取得
				CStage* pStage = (CStage*)pObj;

				// 種類がゴミ箱なら
				if (pStage->GetType() == CStage::TYPE_DUSTBOX)
				{
					// 近くか調べる
					if (CManager::GetColl()->Radius(pStage->GetPos(), CGame::GetField()->GetHeight() * 1.75f, GetPos(), m_fRadius) == true)
					{
						// 振動設定
						pStage->SetShake(true);

						return;
					}
				}
			}
			pObj = pObjNext;
		}
	}
}



//===============================================
// 
// ゴミクラス
// 
//===============================================
//===============================================
// ゴミランダム生成
//===============================================
CInteract* CInteractDust::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;
	// 必要な変数を宣言＆初期化
	const char* pFilename = Filename[0];
	float fWeight = 1.0f;
	TYPE Type = TYPE_BAG;

	// 種類を設定
	int nType;
	nType = rand() % TYPE_MAX;
	Type = (TYPE)nType;

	// 種類ごとにファイル名を代入
	pFilename = Filename[Type];
	// 種類ごとに重さを代入
	fWeight = Weight[Type];


	pInteract->Init(pFilename, pos, rot, fWeight, type);

	return pInteract;
}
//===============================================
// ゴミ生成
//===============================================
CInteract* CInteractDust::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type, CInteractDust::TYPE DustType)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;
	// 必要な変数を宣言＆初期化
	const char* pFilename = Filename[0];
	float fWeight = 1.0f;

	// 種類ごとにファイル名を代入
	pFilename = Filename[DustType];
	// 種類ごとに重さを代入
	fWeight = Weight[DustType];


	pInteract->Init(pFilename, pos, rot, fWeight, type);

	return pInteract;
}

//===============================================
// 
// 雑貨クラス
// 
//===============================================
//===============================================
// 雑貨ランダム生成
//===============================================
CInteract* CInteractGoods::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;
	// 必要な変数を宣言＆初期化
	const char* pFilename = Filename[0];
	float fWeight = 1.0f;
	TYPE Type = TYPE_TISSUE;

	// 種類を設定
	int nType;
	nType = rand() % TYPE_MAX;
	Type = (TYPE)nType;

	// 種類ごとにファイル名を代入
	pFilename = Filename[Type];
	// 種類ごとに重さを代入
	fWeight = Weight[Type];

	pInteract->Init(pFilename, pos, rot, fWeight, type);

	return pInteract;
}
//===============================================
// 雑貨生成
//===============================================
CInteract* CInteractGoods::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type, CInteractGoods::TYPE GoodsType)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;
	// 必要な変数を宣言＆初期化
	const char* pFilename = Filename[0];
	float fWeight = 1.0f;

	// 種類ごとにファイル名を代入
	pFilename = Filename[GoodsType];
	// 種類ごとに重さを代入
	fWeight = Weight[GoodsType];

	pInteract->Init(pFilename, pos, rot, fWeight, type);

	return pInteract;
}

//===============================================
// 
// 花クラス
// 
//===============================================
//===============================================
// 花生成
//===============================================
CInteract* CInteractFlower::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CInteract::TYPE type)
{
	// インタラクトオブジェクト生成
	CInteract* pInteract = new CInteract;
	// 必要な変数を宣言＆初期化
	const char* pFilename = Filename[0];
	float fWeight = 1.0f;
	TYPE Type = TYPE_NONE;

	// 種類を設定
	int nType;
	nType = rand() % TYPE_MAX;
	Type = (TYPE)nType;

	// 種類ごとにファイル名を代入
	pFilename = Filename[Type];
	// 種類ごとに重さを代入
	fWeight = Weight[Type];

	pInteract->Init(pFilename, pos, rot, fWeight, type);

	return pInteract;
}

