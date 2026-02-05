//===============================================
//
// ボスのAI(何の行動をするか決める)[boss.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _BOSSAI_H_
#define _BOSSAI_H_

// インクルード
#include "bossAttack.h"
#include "interact.h"

// ボスAIクラス
class CBossAI
{
public:
	// 理由
	typedef enum
	{
		REASON_NONE = -1,
		REASON_STOP = 0,		// 止まっていた
		REASON_MISS,			// ミスした
		REASON_MOVE,			// 移動距離
		REASON_DUST,			// ゴミについて
		REASON_GOODS,			// 雑貨について
		REASON_MAX
	}REASON;

	// ボスAIのチェック用関数を保持する構造体
	struct CheckItem
	{
		REASON(CBossAI::* func)();   // メンバ関数ポインタ
	};

	// メンバ関数
	CBossAI();
	~CBossAI();
	void Update(void);
	void AddMiss(CInteract::TYPE type);
	void Thrown(void);
	// 取得
	D3DXVECTOR3 GetStayPos(void) { return m_StayPos; }
	int GetNumMiss(void) { return m_nNumMiss; }
	int GetNumAttack(void) { return m_nNumAttack; }
	// 設定
	void SetCoolDown() { m_nCntCoolDown = COOLDOWN; }
private:

	CBossAttack::TYPE DecideAttack(REASON reason);
	CBossAttack::TYPE CheckSituation(REASON reason, CBossAttack::TYPE type);
	// 設定
	void SetAction(REASON reason);

	// ボスAIのチェック用関数
	REASON CheckStop(void);
	REASON CheckMiss(void);
	REASON CheckMoveRange(void);
	REASON CheckStay(void);
	REASON CheckInteractPieces(void);
	REASON CheckInteractGather(void);

	// 定数定義
	static constexpr int COOLDOWN = 180;				// クールダウン時間
	static constexpr int Heal[CBossAttack::TYPE_MAX] =	// 攻撃によるゲージの回復量
	{
		5,	// 投げつけ
		8,	// 薙ぎ払い
		10,	// 火を吐く
		10,	// 雷
		20	// 叩き下ろし
	};
	// 限界地
	static constexpr int SAMEREASON = 3;				// 同じ理由が連続した場合
	static constexpr int ATTACKLIMIT = 10;				// 攻撃数
	static constexpr int MISSLIMIT = 20;				// ミス数
	// 調べる時間
	static constexpr int CHECK_NOATTACK_TIME = 720;		// 攻撃していない
	static constexpr int CHECK_STOP_TIME = 180;			// 止まっていた
	static constexpr int CHECK_MOVERANGE_TIME = 540;	// 移動距離
	static constexpr int CHECK_STAY_TIME = 720;			// 居続けた
	static constexpr int CHECK_INTERACT_TIME = 360;		// インタラクト

	// メンバ変数
	// プレイヤー調査用
	D3DXVECTOR3 m_StayPos;		// 居続けている位置
	CInteract::TYPE m_MissType;	// ミスの種類
	bool m_bMiss;				// ミスしたかどうか
	bool m_bThrown;				// 投げられたかどうか
	int m_nCntMiss;				// ミスした回数	
	// ボス調査用
	REASON m_OldReason;			// 前回の理由
	int m_nCntNoAttackTime;		// 攻撃していない時間計測用
	int m_nCntAttack;			// 攻撃した回数
	int m_nCntCoolDown;			// 攻撃のクールダウン
	int m_nSameReason;			// 攻撃理由が同じだった回数
	// 関数ごとの時間計測用
	int m_nStopTime;			// 移動していない
	int m_nMoveRangeTime;		// 移動距離
	int m_nStayTime;			// 居続けた
	int m_nInteractTime;		// インタラクト
	int m_nGatherTime;			// 収集
	// リザルト表示用
	int m_nNumMiss;				// 最大ミス数
	int m_nNumAttack;			// 最大攻撃数
};

#endif