//==============================================
//
// ボスの攻撃[bossAttack.cpp]
// Author : Kaiti Aoto
//
//==============================================

// インクルード
#include "bossAttack.h"
#include "damegeArea.h"
#include "game.h"
#include "particle.h"
#include "manager.h"

//=================================================
// コンストラクタ
//=================================================
CBossAttack::CBossAttack()
{
	// 値をクリア
	m_pArea = nullptr;
	m_type = TYPE_THROW;
	m_bStay = false;
	m_nCntDamegeTime = 0;
	m_TargetPos = VECTOR3_NULL;
	m_AreaPos = VECTOR3_NULL;
}
//=================================================
// デストラクタ
//=================================================
CBossAttack::~CBossAttack()
{
}
//=================================================
// 投げつけ攻撃
//=================================================
void CBossAttack::Throw(void)
{
	// 攻撃情報
	const int nDamegeTime = 90;
	const int nDamege = 1;

	// 攻撃キャンセル状態じゃないなら
	if (CGame::GetBoss()->GetAttackCancel() == false)
	{
		// 攻撃待機状態じゃないなら
		if (m_bStay == false)
		{
			// ダメージエリアを生成
			CDamegeArea::Create(m_TargetPos, VECTOR3_NULL, 100.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CDamegeArea::TYPE_NONE, nDamege, nDamegeTime);

			m_AreaPos = m_TargetPos;	// エリアの位置をターゲット位置に	

			m_bStay = true;				// 攻撃待機状態にする

			m_nCntDamegeTime = 0;		// タイマーをクリア
		}
		else if (m_bStay == true)
		{
			// タイマーを加算
			m_nCntDamegeTime++;
			// 攻撃終了時
			if (m_nCntDamegeTime >= nDamegeTime)
			{
				m_nCntDamegeTime = 0;	// タイマーをクリア

				// ボスを攻撃しない状態にする
				CGame::GetBoss()->SetAction(false);
				// ボスの攻撃待機を解除
				CGame::GetBoss()->GetAttack()->SetStay(false);
				
				// インタラクト生成の基準位置を設定
				const float fPosX = 25.0f;
				const float fPosY = 500.0f;
				const float fPosZ = 25.0f;

				// インタラクトオブジェクトを増やす
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_GOODS);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_GOODS);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_GOODS);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_GOODS);

				CSound* pSound = CManager::GetSound();
				pSound->PlaySound(CSound::SOUND_LABEL_THROWB);
			}
		}
	}
	// 攻撃キャンセル状態なら
	else if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		// 攻撃キャンセル
		AttackCancel(nDamegeTime);
	}
}
//=================================================
// 薙ぎ払い攻撃
//=================================================
void CBossAttack::Sweep(void)
{
	// 攻撃情報設定
	const int nDamegeTime = 90;
	const int nDamege = 3;

	// 攻撃キャンセル状態なら
	if (CGame::GetBoss()->GetAttackCancel() == false)
	{
		m_TargetPos = CGame::GetBoss()->GetAI()->GetStayPos();	// ダメージエリアの位置を取得


		if (m_bStay == false)
		{
			// フィールドの4分の1を埋めつくすダメージエリアを展開
			CDamegeArea::Create(m_TargetPos, VECTOR3_NULL, CGame::GetField()->GetWidth() / 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), CDamegeArea::TYPE_NONE, nDamege, nDamegeTime);

			m_AreaPos = m_TargetPos;

			// 攻撃待機状態にする
			m_bStay = true;

			m_nCntDamegeTime = 0;
		}
		else if (m_bStay == true)
		{
			// ゴミを増やす
			m_nCntDamegeTime++;
			if (m_nCntDamegeTime >= nDamegeTime)
			{
				m_nCntDamegeTime = 0;

				// ボスを攻撃しない状態にする
				CGame::GetBoss()->SetAction(false);
				// ボスの攻撃待機を解除
				CGame::GetBoss()->GetAttack()->SetStay(false);

				const float fPosX = 25.0f;
				const float fPosY = 500.0f;
				const float fPosZ = 25.0f;

				// ゴミオブジェクトを増やす
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);

				CSound* pSound = CManager::GetSound();
				pSound->PlaySound(CSound::SOUND_LABEL_THROWB);
			}
		}
	}
	else if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		AttackCancel(nDamegeTime);
	}

}
//=================================================
// 火を吐く攻撃
//=================================================
void CBossAttack::Fire(void)
{
	const int nDamegeTime = 420;

	if (CGame::GetBoss()->GetAttackCancel() == false)
	{
		CPlayer* pPlayer = CGame::GetPlayer();
		if (m_bStay == false)
		{
			m_pArea = CDamegeArea::Create(pPlayer->GetPos(), pPlayer->GetRot(), 100.0f, D3DXCOLOR(0.75f, 0.0f, 0.0f, 1.0f), CDamegeArea::TYPE_FIRE, 1, nDamegeTime);
			// 攻撃待機状態にする
			m_bStay = true;
			m_nCntDamegeTime = 0;

			CSound* pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_FIRE);
		}
		else if (m_bStay == true)
		{
			m_nCntDamegeTime++;
			if (m_nCntDamegeTime >= nDamegeTime)
			{
				m_nCntDamegeTime = 0;

				// ボスを攻撃しない状態にする
				CGame::GetBoss()->SetAction(false);
				// ボスの攻撃待機を解除
				CGame::GetBoss()->GetAttack()->SetStay(false);

				m_pArea = nullptr;

				CSound* pSound = CManager::GetSound();
				pSound->Stop(CSound::SOUND_LABEL_FIRE);
			}
		}
	}
	else if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		AttackCancel(nDamegeTime);

		CSound* pSound = CManager::GetSound();
		pSound->Stop(CSound::SOUND_LABEL_FIRE);
	}
}
//=================================================
// 雷攻撃
//=================================================
void CBossAttack::Thunder(void)
{
	const int nDamegeTime = 120;

	if (CGame::GetBoss()->GetAttackCancel() == false)
	{
		CPlayer* pPlayer = CGame::GetPlayer();
		if (m_bStay == false)
		{
			m_pArea = CDamegeArea::Create(pPlayer->GetPos(), pPlayer->GetRot(), 75.0f, D3DXCOLOR(1.0f, 0.25f, 0.0f, 1.0f), CDamegeArea::TYPE_THUNDER, 5, nDamegeTime);
			// 攻撃待機状態にする
			m_bStay = true;

			m_nCntDamegeTime = 0;
		}
		else if (m_bStay == true)
		{
			// 時間を計測
			m_nCntDamegeTime++;

			if (m_nCntDamegeTime >= nDamegeTime)
			{
				m_nCntDamegeTime = 0;

				// ボスを攻撃しない状態にする
				CGame::GetBoss()->SetAction(false);
				// ボスの攻撃待機を解除
				CGame::GetBoss()->GetAttack()->SetStay(false);

				// エリアをNULLにする
				m_pArea = nullptr;

				const float fPosX = 25.0f;
				const float fPosY = 500.0f;
				const float fPosZ = 25.0f;

				// ゴミオブジェクトを増やす
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z + fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x - fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
				CInteract::Create(D3DXVECTOR3(m_AreaPos.x + fPosX, fPosY, m_AreaPos.z - fPosZ), VECTOR3_NULL, CInteract::TYPE_DUST);
			}
			if (m_pArea != nullptr)
			{
				// エリアの位置を保存
				m_AreaPos = m_pArea->GetPos();
			}

			CSound* pSound = CManager::GetSound();
			pSound->PlaySound(CSound::SOUND_LABEL_THUNDER);
		}
	}
	else if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		AttackCancel(nDamegeTime);
	}
}
//=================================================
// 振り下ろし攻撃
//=================================================
void CBossAttack::Slam(void)
{
	const int nDamegeTime = 120;

	// 全インタラクトオブジェクトをごちゃ混ぜに
	if (CGame::GetBoss()->GetAttackCancel() == false)
	{

		m_nCntDamegeTime = 0;
	}
	else if (CGame::GetBoss()->GetAttackCancel() == true)
	{
		AttackCancel(nDamegeTime);
	}
}
//===============================================
// 攻撃キャンセル状態
//===============================================
void CBossAttack::AttackCancel(int nDamegeTime)
{
	// カウンターを加算
	m_nCntDamegeTime++;
	
	// 一定時間経ったら
	if (m_nCntDamegeTime >= nDamegeTime)
	{
		m_nCntDamegeTime = 0;	// カウンターをクリア

		// ボスを攻撃しない状態にする
		CGame::GetBoss()->SetAction(false);
		// ボスの攻撃待機を解除
		CGame::GetBoss()->GetAttack()->SetStay(false);
	}
}
