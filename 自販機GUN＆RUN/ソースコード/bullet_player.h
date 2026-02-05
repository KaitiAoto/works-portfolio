//==============================
//
// 弾[bullet.h]
// Author:kaiti
//
//==============================
#ifndef BULET_H_
#define BULET_H_

#include "main.h"
#include "object.h"
#include "model.h"

#define BULET_SIZE (30)
#define BULLET_LIFE (200)

//オブジェクト2Dクラス
class CBullet:public CObject
{
public:
	//種類
	typedef enum
	{
		USER_PLAYER = 0,
		USER_ENEMY,
		USER_MAX
	}USER;
	typedef enum
	{
		TYPE_A = 0,
		TYPE_B,

		TYPE_MAX
	}TYPE;

	//メンバ関数
	CBullet(int nPriority = 2);
	~CBullet();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user);

	void SetUser(USER user) { m_user = user; };
	int GetUser(void) { return m_user; }
private:
	void Move(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;					//位置(オフセット)
	D3DXVECTOR3 m_rot;					//向き
	D3DXVECTOR3 m_move;					//移動量
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_rotDest;				//目標の向き

	CModel* m_pModel;	//モデルへのポインタ

	int m_nLife;			//寿命
	TYPE m_type;			//種類
	USER m_user;
	bool m_bUse;			//使用しているか
	int m_nPriority;
};

#endif