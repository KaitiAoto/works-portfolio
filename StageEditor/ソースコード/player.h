//==============================
//
// プレイヤー[player.h]
// Author:kaiti
//
//==============================
#ifndef PLAYER_H_
#define PLAYER_H_

#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"

#define PLAYER_SIZE (100)
#define PLAYER_LIFE (20)
#define PLAYER_MODEL (1)
#define PLAYER_SPPE (1.0f)
#define PLAYER_JUMP (5.0f)

//プレイヤークラス
class CPlayer:public CObject
{
public:
	//種類
	typedef enum
	{
		TYPE_A = 0,
		TYPE_B,
		TYPE_MAX
	}TYPE;
	//状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_HIT,
		STATE_ACTION,
		STATE_DEAD,
		STATE_MAX
	}STATE;

	//メンバ関数
	//CPlayer();
	CPlayer(int nPriority = 5);
	~CPlayer();
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,TYPE type);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void SetType(TYPE type) { m_type = type; };
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetUse(void) { m_bUse = true; }
	void SetLanding(bool bLanding) { m_bLanding = bLanding; }
	void SetPosY(float Y) { m_pos.y = Y; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	int GetNumModel(void) { return m_nNumModel; }
	static bool GetUse(void) { return m_bUse; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	int GetLife(void) { return m_nLife; }

	void Hit(const int nDamage);

private:
	void Move(void);
	void Action(void);
	bool Collision(void);
	void State(STATE state);

	//メンバ変数
	D3DXVECTOR3 m_pos;					//位置(オフセット)
	D3DXVECTOR3 m_posOld;				//前回の位置
	D3DXVECTOR3 m_rot;					//向き
	D3DXVECTOR3 m_rotDest;				//目標の向き
	D3DXVECTOR3 m_move;					//移動量
	D3DXVECTOR3 m_size;					//サイズ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス

	int m_nCntState;					//状態カウンター
	int m_nPriority;					//描画順
	STATE m_State;						//状態
	TYPE m_type;						//種類
	static bool m_bUse;					//使用しているか
	bool m_bJump;						//ジャンプしているか
	bool m_bLanding;					//着地判定

	//ステータス
	int m_nLife;						//ライフ
	float m_fSpeed;						//移動速度

	//モデル
	CModel* m_apModel[PLAYER_MODEL];	//モデルへのポインタ
	int m_nNumModel;					//モデル最大数
	CMotion* m_pMotion;					//モーションへのポインタ
};

#endif