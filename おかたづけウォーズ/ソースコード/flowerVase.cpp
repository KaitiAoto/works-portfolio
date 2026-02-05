//===============================================
//
// 花瓶処理[flowerVase.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "flowerVase.h"
#include "manager.h"
#include "particle.h"

// 静的メンバ変数
int CFlowerVase::m_nNumBouquet = 0;

//===============================================
// コンストラクタ
//===============================================
CFlowerVase::CFlowerVase(int nPriority) : CObjectX(nPriority)
{
}
//===============================================
// デストラクタ
//===============================================
CFlowerVase::~CFlowerVase()
{
}
//===============================================
// 生成処理
//===============================================
CFlowerVase* CFlowerVase::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// インスタンス生成
	CFlowerVase* pInteract = new CFlowerVase;
	
	//初期化
	if (FAILED(pInteract->Init(pos, rot)))
	{
		delete pInteract;
		return nullptr;
	}

	return pInteract;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CFlowerVase::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{	
	// ファイル名
	const char* pFilename = Filename[0];

	// 親クラスの初期化
	CObjectX::Init(pFilename, pos, rot);

	// 花束アリのモデルを生成し見えなくする
	m_pBouquet = CObjectX::Create(Filename[1], pos, rot);
	m_pBouquet->SetDraw(false);

	// 位置を少しずらす
	pos.y += 0.1f;

	// 円生成＆見えなくする
	m_pCircle = CMeshCircle::Create(pos, rot, Radius, D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f), CMeshCircle::TYPE_OUT);
	m_pCircle->SetUse(false);

	// 影生成
	CShadow::Create(pos, rot, Radius, Radius);

	// オブジェクトの種類設定
	SetObjType(TYPE_FLOWERVASE);	// オブジェクトのタイプ

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CFlowerVase::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CFlowerVase::Update(void)
{
	// 親クラスの更新
	CObjectX::Update();

	// 当たり判定
	CollRadius();
	CollObjPhysics();

	// 花束モデルを描画するか設定
	m_pBouquet->SetDraw(m_bHaveBouquet);
}
//===============================================
// 描画処理
//===============================================
void CFlowerVase::Draw(void)
{
	// 親クラスの描画
	CObjectX::Draw();
}
//===============================================
// プレイヤーが花束有りの時のリアクション
//===============================================
void CFlowerVase::Reaction(void)
{
	D3DXVECTOR3 scale = GetScale();	// スケール取得

	// スケール加算
	scale.x += 0.01f;
	scale.y += 0.01f;
	scale.z += 0.01f;

	// スケールが一定以上になったら
	if (scale.x >= 1.5f)
	{
		CParticle::Create(GetPos(), GetRot(), D3DXCOLOR(1.0f, 0.25f, 1.0f, 1.0f), 5, 15.0f, CParticle::TYPE_NONE);	// パーティクル生成
		// スケールを戻す
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

	// スケール設定
	SetScale(scale);
}
//===============================================
// 半径での判定処理
//===============================================
void CFlowerVase::CollRadius(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	
	// 花束を持っていて且つ花が挿されていなければ
	if (pPlayer->GetHaveBouquet() == true && m_bHaveBouquet == false)
	{
		// 位置＆サイズ取得
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
		D3DXVECTOR3 PlayerSize = pPlayer->GetSize();

		// 半径をサイズから算出
		float fPlayerRadius = max(PlayerSize.x, max(PlayerSize.y, PlayerSize.z)) * 0.5f;

		// 半径での判定
		if (CManager::GetColl()->Radius(PlayerPos, fPlayerRadius, GetPos(), Radius) == true)
		{// 範囲内なら
			// キー取得
			CInputKey* pInputKey = CManager::GetInputKey();
			// パッド
			CInputPad* pInputPad = CManager::GetInputPad();

			// 設置
			if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputPad->GetTrigger(CInputPad::JOYKEY_A) == true)
			{
				pPlayer->SetBouqet(false);					// 花束を持っていないことにする
				pPlayer->GetFlowerCnt()->ResetHaveFlower();	// 花数カウンターをリセット
				m_bHaveBouquet = true;						// 花束を持っていることにする
				m_nNumBouquet++;							// 花束回数加算

				// 音
				CSound* pSound = CManager::GetSound();
				pSound->PlaySound(CSound::SOUND_LABEL_SETBOUQUWT);
			}
			// 円を描画
			m_pCircle->SetUse(true);
		}
		else
		{
			// 円を描画しない
			m_pCircle->SetUse(false);
		}

		// リアクション
		Reaction();
	}
	// 花瓶に花が挿してあれば
	else if (m_bHaveBouquet == true)
	{
		// ボス情報取得
		CBoss* pBoss = CGame::GetBoss();
		// 位置＆サイズ取得
		D3DXVECTOR3 BossPos = pBoss->GetPos();
		D3DXVECTOR3 BossSize = pBoss->GetSize();
		// 半径をサイズから算出
		float fBossRadius = max(BossSize.x, max(BossSize.y, BossSize.z)) * 0.5f;

		// 半径での判定
		if (CManager::GetColl()->Radius(BossPos, fBossRadius, GetPos(), Radius * 2.0f) == true)
		{// 範囲内なら
			m_bHaveBouquet = false;			// 花束を持っていない
			pBoss->SetAttackCancel(false);	// 攻撃キャンセル解除
		}

		// 円を描画しない
		m_pCircle->SetUse(false);
	}
	// プレイヤーが花束を持っていない
	if (pPlayer->GetHaveBouquet() == false)
	{
		// スケールを戻す
		SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
}
//===============================================
// OBBでの判定処理
//===============================================
void CFlowerVase::CollObjPhysics(void)
{

}
