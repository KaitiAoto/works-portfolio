//============================
//
// モデル表示処理[objectX.h]
// Author:kaiti
//
//============================
#ifndef _OBJECTX_H
#define _OBJECTX_H_

#include "main.h"
#include "object.h"

//クラス
class CObjectX :public CObject
{
public:

	//メンバ関数
	CObjectX();
	//仮想関数
	~CObjectX();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:	
	int* m_nIdxTex;				//テクスチャインデックスへのポインタ
	LPD3DXMESH m_pMesh;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアルへのポインタ
	DWORD m_dwNumMat;			//マテリアルの数
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
};

#endif
