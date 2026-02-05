//==============================
//
// 敵[enemy.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _ENEMY_H_
#define _ENEMY_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "gauge_enemy.h"
#include "enemy_group.h"

// 
class CShadow;

// マクロ定義
#define ENEMY_LIFE (30)
#define ENEMY_ACTIONSTATE (300)
#define ENEMY_SPEED (1.35f)

// 敵クラス
class CEnemy :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_LIGHTER = 0,
		TYPE_MONEY0,
		TYPE_MONEY1,
		TYPE_MAGNET,
		TYPE_TORNADO,
		TYPE_SPRAY,
		TYPE_EXT,
		TYPE_KETTLE,
		TYPE_RANGE,
		TYPE_MAX
	}TYPE;
	// 状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_STAY,
		STATE_MOVE,
		STATE_HIT,
		STATE_ACTION,
		STATE_DEAD,
		STATE_MAX
	}STATE;

	// メンバ関数
	CEnemy(int nPriority = PRIORITY_ENEMY);
	~CEnemy();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(const int nDamage);
	void MovePos(D3DXVECTOR3 move) { m_pos += move; }

	// 設定
	void SetType(TYPE type) { m_type = type; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	// 取得
	TYPE GetType(void) { return m_type; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	

	// 静的メンバ関数
	static CEnemy* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);
	static CEnemy* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, CEnemyGroup* Group);

private:
	void Move(void);
	void Move_Normal(void);
	void Move_Charge(void);
	void Move_Jump(void);
	void Move_Shot(void);
	

	void State(STATE state);
	bool Collision(void);
	const char* SetModelName(void);
	void ItemSet();

	// メンバ変数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_posHalf;		// 半分の高さの位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;			// 移動量
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_rotDest;		// 目標の向き
	D3DXVECTOR3 m_size;			// サイズ

	CModel* m_pModel;			// モデルへのポインタ
	CShadow* m_pShadow;			// 影へのポインタ
	int m_nLife;				// 寿命
	int m_nCntState;			// 状態カウンター
	int m_nStayCounter;			// 滞在状態のカウント
	bool m_bUse;				// 使用しているか
	float m_fRandomAngle;		// ランダム方向の角度
	TYPE m_type;				// 種類
	STATE m_State;				// 
	CEnemyGauge* m_pGauge;		// ゲージへのポインタ
	CEnemyGroup* m_pMyGroup;	// グループへのポインタ

	//
	static int m_nCntKill;
};

#endif