//===============================================
//
// ボス[boss.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _BOSS_H_
#define _BOSS_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "bossAttack.h"
#include "bossAI.h"
#include "gauge_billboard.h"
#include "gaugeModel.h"

// 敵クラス
class CBoss :public CObject
{
public:
	// モーションの種類
	typedef enum
	{
		MOTION_NEUTRAL = 0,	// ニュートラル
		MOTION_MOVE,		// 移動
		MOTION_THROW,		// 投げ
		MOTION_SWEEP,		// 薙ぎ払い
		MOTION_FIRE,		// 火炎放射
		MOTION_THUNDER,		// 雷
		MOTION_SLAM,		// たたきおろし
		MOTION_MAX,
	}MOTION;

	// 状態
	typedef enum
	{
		STATE_NONE = 0,	// 通常
		STATE_MOVE,		// 移動
		STATE_ACTION,	// 攻撃
		STATE_DEAD,		// 死亡
		STATE_MAX
	}STATE;

	// メンバ関数
	CBoss(int nPriority = static_cast<int>(Priority::Character));
	~CBoss();
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(const int nDamage);
	void Heal(int nHeal) { m_nLife += nHeal; if (m_nLife >= BOSS_LIFE) { m_nLife = BOSS_LIFE; } }
	void MovePos(D3DXVECTOR3 move) { m_pos += move; }
	void State(STATE state);
	void EndSave(void);

	// 設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetAction(bool bAction);
	void SetAttackCancel(bool bCancel) { m_bAttackCancel = bCancel; m_nCntAttack--; }
	// 取得
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	CGaugeModel* GetGauge(void) { return m_pGaugeM; }
	bool GetAction(void) { return m_bAction; }
	bool GetAttackCancel(void) { return m_bAttackCancel; }
	int GetLife(void) { return m_nLife;}

	// 静的メンバ関数
	static CBoss* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	static CBossAttack* GetAttack(void) { return m_pAttack; }
	static CBossAI* GetAI(void) { return m_pAI; }
	static int GetMaxLife(void) { return BOSS_LIFE; }
private:

	void Move(void);
	void Action(void);
	void ChekFlowerVase(void);
	
	// 定数定義
	static constexpr int BOSS_MODEL = 8;			// モデル数
	static constexpr int BOSS_LIFE = 50;			// HP
	static constexpr float BOSS_SPEED = 5.35f;		// 移動速度

	// メンバ変数
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_posOld;			// 前回の位置
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotDest;			// 目標の向き
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_size;				// サイズ
	// モデル関連
	CModel* m_apModel[BOSS_MODEL];	// モデルへのポインタ
	CMotion* m_pMotion;				// モーションへのポインタ
	int m_nNumModel;				// モデル最大数

	CGaugeModel* m_pGaugeM;			// モデルゲージへのポインタ
	int m_nLife;					// 寿命
	int m_nCntState;				// 状態カウンター
	bool m_bUse;					// 使用しているか
	bool m_bAction;					// アクションしているかどうか
	bool m_bAttackCancel;			// 攻撃キャンセル状態か
	STATE m_State;					// 状態 

	// 静的メンバ変数
	static int m_nMoveTarget;		// 移動目標のインデックス番号 
	static int m_nCntAttack;		// 攻撃回数計測用
	static CBossAttack* m_pAttack;	// ボス攻撃へのポインタ
	static CBossAI* m_pAI;			// ボスAIへのポインタ
};

#endif