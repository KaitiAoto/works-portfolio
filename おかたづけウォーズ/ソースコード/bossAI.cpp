//===============================================
//
// ボスのAI(何の行動をするか決める)[boss.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "bossAI.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "gatherArea.h"

//===============================================
// コンストラクタ
//===============================================
CBossAI::CBossAI()
{
	// 値をクリア
	m_nCntMiss = 0;
	m_bMiss = false;
	m_bThrown = false;
	m_StayPos = VECTOR3_NULL;
	m_nCntNoAttackTime = 0;
	m_nCntAttack = 0;
	m_nCntCoolDown = 0;
	m_nSameReason = 0;
	m_MissType = CInteract::TYPE_DUST;
	m_OldReason = REASON_NONE;

	// 関数ごとの時間計測用変数クリア
	m_nStopTime = 0;
	m_nMoveRangeTime = 0;
	m_nStayTime = 0;
	m_nInteractTime = 0;
	m_nGatherTime = 0;

	// リザルト表示用
	m_nNumMiss = 0;
	m_nNumAttack = 0;
}
//===============================================
// デストラクタ
//===============================================
CBossAI::~CBossAI()
{
}
//===============================================
// 更新処理
//===============================================
void CBossAI::Update(void)
{
	// デバッグフォント
	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("ミス数：%d", m_nNumMiss);
	pDegub->Print("攻撃数：%d", m_nNumAttack);

	// クールダウン中なら
	if (m_nCntCoolDown > 0)
	{
		pDegub->Print("!!!!クールダウン!!!!");

		// カウントを減らす
		m_nCntCoolDown--;
		// カウントが０以下なら
		if (m_nCntCoolDown <= 0)
		{
			m_nCntCoolDown = 0;	// カウンターをクリア
		}
		return;	// そのまま終わる
	}

	// 攻撃中なら何もせずカウントだけ更新
	if (CGame::GetBoss()->GetAction())
	{
		m_nCntNoAttackTime = 0;	// 攻撃していない時間を０にする
		return;	// そのまま終わる
	}

	// 攻撃していない時間を加算
	m_nCntNoAttackTime++;

	// 上から順にチェックする関数のリスト
	static const CheckItem CheckList[] =
	{
		{ &CBossAI::CheckStop},				// 止まっていないか
		{ &CBossAI::CheckStay},				// どの辺にいるか
		{ &CBossAI::CheckMoveRange},		// 移動距離
		{ &CBossAI::CheckMiss},				// ミスしていないか
		{ &CBossAI::CheckInteractGather},	// インタラクトの収集に関して
		{ &CBossAI::CheckInteractPieces},	// インタラクトの数に関して
	};

	// 登録されている全てのチェック処理を調べる
	for (auto& item : CheckList)
	{
		//item.func が指すチェックする関数を呼ぶ
		REASON reason = (this->*item.func)();
		// 何か攻撃するなら
		if (reason != REASON_NONE)
		{
			// 攻撃設定
			SetAction(reason);
			return;
		}
	}

	// 一定時間攻撃していないときの処理
	if (m_nCntNoAttackTime >= CHECK_NOATTACK_TIME)
	{

	}
}
//===============================================
// ミスを加算
//===============================================
void CBossAI::AddMiss(CInteract::TYPE type)
{
	// ゲームなら
	if (CGame::GetType() == CGame::TYPE_GAME)
	{
		m_bMiss = true;		// ミスした 
		m_nCntMiss++;		// カウンターを加算
		m_MissType = type;	// ミスの種類を代入
		m_nNumMiss++;		// ミスの総数を加算
	}
}
//===============================================
// 攻撃設定
//===============================================
void CBossAI::SetAction(REASON reason)
{
	// 攻撃していなければ
	if (CGame::GetBoss()->GetAction() == false)
	{
		// 攻撃方法決定
		CBossAttack::TYPE type = DecideAttack(reason);

		// 攻撃の種類を設定
		CGame::GetBoss()->GetAttack()->SetType(type);

		// 攻撃の種類別にボスのHP回復
		int nHeal = Heal[type];
		// 攻撃キャンセル時
		if (CGame::GetBoss()->GetAttackCancel() == true)
		{
			// ゲージ増加量を半分に
			nHeal /= (int)2.0f;
		}
		// ゲージ増加
		CGame::GetBoss()->Heal(nHeal);
		// ボスを攻撃している状態にする
		CGame::GetBoss()->SetAction(true);
		
		m_nCntAttack++;	// 攻撃回数を加算
		m_nNumAttack++;	// 最大攻撃回数を加算
	}
}
//===============================================
// なんの攻撃をするか決める
//===============================================
CBossAttack::TYPE CBossAI::DecideAttack(REASON reason)
{
	CBossAttack::TYPE type = CBossAttack::TYPE_NONE;	// 攻撃種類設定用変数宣言＆初期化

	// 理由ごとに攻撃方法を設定
	switch (reason)
	{
		// 止まっていた
	case REASON_STOP:
		type = CBossAttack::TYPE_THUNDER;	// 火炎放射
		break;
		// ミスした
	case REASON_MISS:
		type = CBossAttack::TYPE_SWEEP;	// 投げつけ
		break;
		// 移動距離
	case REASON_MOVE:
		type = CBossAttack::TYPE_SWEEP;	// 薙ぎ払い
		break;
		// ゴミに関して
	case REASON_DUST:
		type = CBossAttack::TYPE_THROW;	// 投げつけ
		break;
		// 雑貨に関して
	case REASON_GOODS:
		type = CBossAttack::TYPE_THROW;	// 投げつけ
		break;
	default:
		break;
	}

	// 現在の状況を調べたうえで値を返す
	return CheckSituation(reason, type);
}
//===============================================
// 現在の状況を調べる
//===============================================
CBossAttack::TYPE CBossAI::CheckSituation(REASON reason, CBossAttack::TYPE type)
{
	CBossAttack::TYPE NewType = type;	// 攻撃方法保存

	// 攻撃回数が限界地より多ければ
	if (m_nCntAttack >= ATTACKLIMIT)
	{
		// ゲージの最大量を基準に今のゲージから割合を算出

	}

	//-------------------------------------------
	// 同じ攻撃理由が続いたら
	//-------------------------------------------
	// 攻撃理由が前回と同じ
	if (m_OldReason == reason && reason != REASON_NONE)
	{
		m_nSameReason++;	// 同じ理由だった回数を加算
	}
	// 前回と違う理由なら
	else
	{
		m_nSameReason = 0;	// 同じ理由だった回数を0に
	}

	// 今回の攻撃理由を保存
	m_OldReason = reason;	

	// 同じ理由が連続したら
	if (m_nSameReason >= SAMEREASON - 1)
	{
		// 強力な攻撃にする
		// 雑貨が理由なら
		if (reason == REASON_GOODS)
		{
			NewType = CBossAttack::TYPE_FIRE;		// 火炎放射
		}
		// ゴミが理由なら
		else if (reason == REASON_DUST)
		{
			NewType = CBossAttack::TYPE_THUNDER;	// 雷
		}
		// それ以外
		else
		{
			NewType = CBossAttack::TYPE_SWEEP;		// 薙ぎ払い
		}

		m_nSameReason = 0;	// カウンターをクリア
	}


	return NewType;	// 値を返す
}
//===============================================
// 投げられた
//===============================================
void CBossAI::Thrown(void)
{
}
//===============================================
// 止まっていないか調べる
//===============================================
CBossAI::REASON CBossAI::CheckStop(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	bool bPlayerMove = pPlayer->GetbMove();	// プレイヤーが移動しているかを取得

	// 一切動かなかったら
	if (bPlayerMove == false)
	{
		// 時間を計測
		m_nStopTime++;
		// 一定時間動かなかったら
		if (m_nStopTime >= CHECK_STOP_TIME)
		{
			m_nStopTime = 0;	// タイマーをクリア

			// 理由を返す
			return REASON_STOP;
		}
	}
	// 動いていたら
	else if (bPlayerMove == true)
	{
		// 時間をクリア
		m_nStopTime = 0;
	}

	// 攻撃しない
	return REASON_NONE;
}
//===============================================
// ミスを調べる
//===============================================
CBossAI::REASON CBossAI::CheckMiss(void)
{
	// ミスしていれば
	if (m_bMiss == true)
	{
		m_bMiss = false;	// ミスしていないことにする

		if (m_nCntMiss >= MISSLIMIT)
		{
			m_nCntMiss = 0;
			return REASON_MISS;
		}

		// ゴミに関するミスなら
		if (m_MissType == CInteract::TYPE_DUST)
		{
			// 理由を返す
			return REASON_DUST;
		}
		// 雑貨に関するミスなら
		else if (m_MissType == CInteract::TYPE_GOODS)
		{
			// 理由を返す
			return REASON_GOODS;
		}
	}

	// 攻撃しない
	return REASON_NONE;
}
//===============================================
// どれくらい移動しているか調べる
//===============================================
CBossAI::REASON CBossAI::CheckMoveRange(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();				// プレイヤーの今現在の位置

	static D3DXVECTOR3 PlayerBasePos = pPlayer->GetPos();	// 調べる基準地点
	static const float fRadius = 200.0f;					// クリアに必要な移動範囲
	static bool bReset = false;								// リセットするかどうか

	REASON reason = REASON_NONE;	// 理由設定用変数宣言＆初期化

	// ＸＺ軸の移動量を調べる
	if (PlayerPos.x >= PlayerBasePos.x + fRadius || PlayerPos.x <= PlayerBasePos.x - fRadius 
	 || PlayerPos.z >= PlayerBasePos.z + fRadius || PlayerPos.z <= PlayerBasePos.z - fRadius)
	{// 今回の位置が基準値を上回っていれば
		bReset = true;	// リセットする
	}

	// リセットしないなら
	if (bReset == false)
	{
		// 時間を計測
		m_nMoveRangeTime++;

		// 一定時間経過
		if (m_nMoveRangeTime >= CHECK_MOVERANGE_TIME)
		{// 一定時間近い距離に居続けたら

			bReset = true;	// リセットする

			// 理由を設定
			reason = REASON_MOVE;
		}
	}
	// リセットする
	else if (bReset == true)
	{
		m_nMoveRangeTime = 0;		// タイマーを０にする
		PlayerBasePos = PlayerPos;	// 基準地点を現在の位置に
		bReset = false;				// リセットしないようにする
	}

	// 理由を返す
	return reason;
}
//===============================================
// どの辺にいるか調べる
//===============================================
CBossAI::REASON CBossAI::CheckStay(void)
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();	// プレイヤーの今現在の位置

	// フィールドの情報取得
	D3DXVECTOR3 FieldPos = CGame::GetField()->GetPos();
	float fFieldWidth = CGame::GetField()->GetWidth() / 2.0f;	// フィールドの横幅
	float fFieldHeight = CGame::GetField()->GetHeight() / 2.0f;	// フィールドの奥行

	D3DXVECTOR3 OldStayPos = m_StayPos;	// 居続けた位置を保存

	// ステージ4分割の座標
	static D3DXVECTOR3 CornerPos[4] =
	{
		{ FieldPos.x - fFieldWidth, FieldPos.y, FieldPos.z - fFieldHeight },	// 左下
		{ FieldPos.x - fFieldWidth, FieldPos.y, FieldPos.z + fFieldHeight },	// 左上
		{ FieldPos.x + fFieldWidth, FieldPos.y, FieldPos.z - fFieldHeight },	// 右下
		{ FieldPos.x + fFieldWidth, FieldPos.y, FieldPos.z + fFieldHeight },	// 右上
	};

	// フィールドを４分割した時どこにいるか調べる
	// 左下
	if (PlayerPos.x <= FieldPos.x && PlayerPos.z <= FieldPos.z)
	{
		m_StayPos = CornerPos[0];	// 居続けた位置を設定
	}
	// 左上
	else if (PlayerPos.x <= FieldPos.x && PlayerPos.z > FieldPos.z)
	{
		m_StayPos = CornerPos[1];	// 居続けた位置を設定
	}
	// 右下
	else if (PlayerPos.x > FieldPos.x && PlayerPos.z <= FieldPos.z)
	{
		m_StayPos = CornerPos[2];	// 居続けた位置を設定
	}
	// 右上
	else if (PlayerPos.x > FieldPos.x && PlayerPos.z > FieldPos.z)
	{
		m_StayPos = CornerPos[3];	// 居続けた位置を設定
	}

	// 一定時間同じ範囲にいたら
	if (m_StayPos == OldStayPos)
	{
		// 時間を計測
		m_nStayTime++;

		// 一定時間経過
		if (m_nStayTime >= CHECK_STAY_TIME)
		{
			m_nStayTime = 0;	// タイマーをクリア

			// 理由を返す
			return REASON_MOVE;
		}
	}
	// 違う範囲に行ったら
	else
	{
		m_nStayTime = 0;	// タイマーをクリア
	}

	// 攻撃しない
	return REASON_NONE;
}
//===============================================
// インタラクトオブジェクトの総数の変化を調べる
//===============================================
CBossAI::REASON CBossAI::CheckInteractPieces(void)
{
	static const int nDiff = 3;							// 個数の差
	static int nOldDust = CInteract::GetNumDust();		// 基準となるゴミの数
	static int nOldGoods = CInteract::GetNumGoods();	// 基準となる雑貨の数
	
	// 時間を計測
	m_nInteractTime++;

	REASON reason = REASON_NONE;	// 理由設定用変数宣言＆初期化

	// 一定時間経過
	if (m_nInteractTime >= CHECK_INTERACT_TIME)
	{
		//-----------------------------
		// ゴミ
		//-----------------------------
		int nNowDust = CInteract::GetNumDust();
		
		// 基準より増えていた場合
		if (nOldDust <= nNowDust)
		{
			reason = REASON_DUST;
		}
		// 差が小さい場合（基準と比べ変化が少ない場合）
		else if (nOldDust - nDiff <= nNowDust)
		{
			reason = REASON_DUST;
		}

		//-----------------------------
		// 雑貨
		//-----------------------------
		int nNowGoods = CInteract::GetNumGoods();
		
		// 基準より減っていた場合
		if (nOldGoods > nNowGoods)
		{
			reason = REASON_GOODS;
		}

		// タイマーをリセット
		m_nInteractTime = 0;

		// 基準の個数を更新
		nOldDust = nNowDust;
		nOldGoods = nNowGoods;
	}

	return reason;	// 理由を返す
}
//===============================================
// インタラクトオブジェクトの収集について調べる
//===============================================
CBossAI::REASON CBossAI::CheckInteractGather(void)
{
	static const int nDustDiff = 3;		// ゴミの個数の差
	static const int nGoodsDiff = 5;	// 雑貨の個数の差

	static int nOldDust = CGatherArea::GetDustIn();		// 基準となるゴミの数
	static int nOldGoods = CGatherArea::GetGoodsIn();	// 基準となる雑貨の数

	m_nGatherTime++;	// 理由設定用変数宣言＆初期化

	// 値を返す用の変数
	REASON reason = REASON_NONE;

	// 一定時間経過
	if (m_nGatherTime >= CHECK_INTERACT_TIME)
	{
		//-----------------------------
		// 雑貨
		//-----------------------------
		int nNowGoods = CGatherArea::GetGoodsIn();
		// 基準より増えていた場合
		if (nOldGoods <= nNowGoods)
		{
			// 攻撃しない
			reason = REASON_NONE;
		}
		// 減っていた
		else if (nOldGoods > nNowGoods)
		{
			// 攻撃しない
			reason = REASON_GOODS;
		}
		// 差が小さい場合（基準と比べ変化が少ない場合）
		if (nOldGoods - nGoodsDiff <= nNowGoods)
		{
			reason = REASON_GOODS;
		}

		//-----------------------------
		// ゴミ
		//-----------------------------
		int nNowDust = CGatherArea::GetDustIn();
		// ゴミがあれば
		if (nNowDust > 0)
		{
			// 基準より増えていた場合
			if (nOldDust <= nNowDust)
			{
				reason = REASON_DUST;
			}
			// 差が小さい場合（基準と比べ変化が少ない場合）
			else if (nOldDust - nDustDiff <= nNowDust)
			{
				reason = REASON_DUST;
			}
		}

		// 攻撃しなければ
		if (reason == REASON_NONE)
		{
			// ボスにダメージ
			CGame::GetBoss()->Hit(3);
		}

		// タイマーをリセット
		m_nGatherTime = 0;
		
		// 基準の個数を更新
		nOldDust = nNowDust;
		nOldGoods = nNowGoods;
	}

	return reason;	// 理由を返す
}