//==============================
//
// 弾[bullet.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _BULET_H_
#define _BULET_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "effect.h"

// マクロ定義
#define BULET_SIZE (30)
#define BULLET_LIFE (200)
#define BULLET_ATTACK (10)
#define BULLET_SPEED (3.5f)

// 弾クラス
class CBullet:public CObject
{
public:
	// ユーザー
	typedef enum
	{
		USER_PLAYER = 0,
		USER_ENEMY,
		USER_MAX
	}USER;
	// 種類
	typedef enum
	{
		TYPE_CAN = 0,
		TYPE_ICE,
		TYPE_CAPSULE,
		TYPE_PETBOTTLE,
		TYPE_DUST,
		TYPE_SNACK,
		TYPE_CIGARET,
		TYPE_CARD,
		TYPE_BOTTLE,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CBullet(int nPriority = PRIORITY_BULLET);
	~CBullet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	int GetUser(void) { return m_user; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	TYPE GetType(void) { return m_type; }
	bool GetUse(void) { return m_bUse; }
	bool GetSkill(void) { return m_bSkill; }
	// 設定
	void SetUser(USER user) { m_user = user; };
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetSkill(bool bSkill) { m_bSkill = bSkill; }

	// 静的メンバ関数
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user);

private:
	void Move(void);
	void MoveBounce(void);
	bool Collision(void);
	const char* SetPlayerBullet(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;					// 位置(オフセット)
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_rotDest;				// 目標の向き
	D3DXVECTOR3 m_size;					// サイズ

	CModel* m_pModel;					// モデルへのポインタ
	int m_nLife;						// 寿命
	TYPE m_type;						// 種類
	USER m_user;						// ユーザー
	bool m_bUse;						// 使用しているか
	bool m_bSkill;						// スキルかどうか
};

#endif