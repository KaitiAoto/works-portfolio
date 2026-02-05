//===============================================
//
// プレイヤー[player.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "flowerCounter.h"
#include "shadow.h"
#include "shadowS.h"
#include "meshcylinder.h"
#include "gauge_billboard.h"
#include "gaugeModel.h"
#include "throwLockIcon.h"

// プレイヤークラス
class CPlayer:public CObject
{
public:
	// モーションの種類
	typedef enum
	{
		MOTION_NEUTRAL = 0,
		MOTION_MOVE,
		MOTION_ACTION,
		MOTION_JUMP,
		MOTION_LANDING,
		MOTION_MAX,
	}MOTION;

	// 状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_HIT,
		STATE_ACTION,
		STATE_STUN,
		STATE_DEAD,
		STATE_MAX
	}STATE;


	// メンバ関数
	CPlayer(int nPriority = static_cast<int>(Priority::Character));
	~CPlayer();
	HRESULT Init(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Heal(int nHeal) { m_nLife += nHeal; if (m_nLife >= PLAYER_LIFE) { m_nLife = PLAYER_LIFE; } }
	void AddSpeed(float fAddSpeed) { m_fSpeed += fAddSpeed; }
	void Hit(const int nDamage);
	void Return(void) { m_pos = m_posOffSet; }
	void UpdateOBB(void);

	// 設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetTargetPos(D3DXVECTOR3 pos) { m_TargetPos = pos; }
	void SetUse(void) {m_bUse = true; m_nLife = PLAYER_LIFE;}
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetBouqet(bool bHave) { m_bHaveBouquet = bHave; }
	void SetStun(int nStunTime) { m_nStunTime = nStunTime; State(STATE_STUN); }
	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetMove(void) { return m_moveOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	bool GetbMove(void) { return m_bMove; }
	bool GetHaveBouquet(void) { return m_bHaveBouquet; }
	int GetLife(void) { return m_nLife; }
	int GetStayTime(void) { return m_nStayTime; }
	float GetSpeed(void) { return m_fSpeed; }
	Obb GetOBB(void) { return m_obb; }
	CFlowerCnt* GetFlowerCnt(void) { return m_pFlowerCnt; }

	// 静的メンバ関数
	static CPlayer* Create(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static bool GetUse(void) { return m_bUse; }

private:
	void Move(void);
	void Action(void);
	void Throw(void);
	bool Collision(void);
	void State(STATE state);
	void Stun(void);
	void CheckStack(bool bColl, bool bMove);
	void RecoverFromStuck(void);
	void Blink(void);

	// 定数定義
	static constexpr int PLAYER_LIFE = 20;			// HP
	static constexpr int PLAYER_MODEL = 10;			// モデル数
	static constexpr float PLAYER_SPEED = 3.0f;		// 移動速度
	static constexpr float PLAYER_RADIUS = 30.0f;	// 半径

	// メンバ変数
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posOld;				// 前回の位置
	D3DXVECTOR3 m_posOffSet;			// 位置(オフセット)
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_rotDest;				// 目標の向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_moveOld;				// 移動量
	D3DXVECTOR3 m_size;					// サイズ
	D3DXVECTOR3 m_TargetPos;			// 目標の位置
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXCOLOR m_BaseColor;				// 通常の色
	D3DXCOLOR m_DamageColor;			// ダメージ時の色
	CGaugeModel* m_pGaugeM;				// モデルゲージへのポインタ
	CFlowerCnt* m_pFlowerCnt;			// 花数カウンターへのポインタ
	CShadow* m_pShadow;					// 影へのポインタ
	CMeshCylinder* m_pThrowRange;		// 投げる範囲可視化用円柱へのポインタ
	CThrowLockIcon* m_pThrowLockIcon;	// 投げられない時用アイコンへのポインタ
	STATE m_State;						// 状態
	int m_nCntState;					// 状態カウンター
	int m_nCntStackt;					// スタックカウント
	int m_nStayTime;					// 待機時間計測用
	int m_nStunTime;					// スタン時間計測用
	bool m_bMove;						// 移動しているか
	bool m_bHaveBouquet;				// 花束を持っているか
	bool m_bStun;						// スタンしているか
	// ステータス
	int m_nLife;						// ライフ
	float m_fSpeed;						// 移動速度
	// モデル
	CModel* m_apModel[PLAYER_MODEL];	// モデルへのポインタ
	CMotion* m_pMotion;					// モーションへのポインタ
	int m_nNumModel;					// モデル最大数
	Obb m_obb;							// OBB

	// 静的メンバ変数
	static bool m_bUse;					// 使用しているか
};

#endif