//==============================
//
// オブジェクト[object.h]
// Author:kaiti
//
//==============================
#ifndef OBJECT_H_
#define OBJECT_H_

#include "main.h"

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)//座標・法線・カラー・テクスチャ

#define MAX_OBJECT (512)
#define NUM_PRIORITY (8)

typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

//オブジェクトクラス
class CObject
{
public:
	//頂点情報(3D)の構造体
	typedef struct
	{
		D3DXVECTOR3 pos;//頂点座標
		D3DXVECTOR3 nor;//法線ベクトル
		D3DCOLOR col;   //頂点カラー
		D3DXVECTOR2 tex;//テクスチャ座標
	}VERTEX_3D;

	//種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_ENEMY_BASE,
		TYPE_BULLET,
		TYPE_VENDER,
		TYPE_SHADOW,
		TYPE_STAGE,
		TYPE_FIELD,
		TYPE_BG,
		TYPE_EXPLOSION,
		TYPE_EFFECT,
		TYPE_NUMBER,
		TYPE_SCORE,
		TYPE_TIME,
		TYPE_RESTOCK,
		TYPE_BULLETCNT,
		TYPE_GAUGE,
		TYPE_WALL,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	//CObject();
	CObject(int nPriority = 3);
	//仮想関数
	virtual ~CObject();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	//純粋仮想関数
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	//void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject* GetObj(int nPriority, int nIdx) { return m_apObject[nPriority][nIdx]; }

	void SetObjType(TYPE type) { m_Objtype = type; }
	TYPE GetObjType(void) { return m_Objtype; }
	int GetPrio(void) { return m_nPriority; }

protected:
	void Release(void);

private:

	static CObject* m_apObject[NUM_PRIORITY][MAX_OBJECT];	//オブジェクトへのポインタ
	static int m_nNumAll;					//最大数
	int m_nId;								//自身のID
	TYPE m_Objtype;							//種類
	int m_nPriority;
};

#endif