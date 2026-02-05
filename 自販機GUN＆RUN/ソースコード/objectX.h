//============================
//
// モデル表示処理[objectX.h]
// Author : Kaiti Aoto
//
//============================

// 二重インクルード防止
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

// インクルード防止
#include "main.h"
#include "object.h"

// オブジェクトXクラス
class CObjectX :public CObject
{
public:

	// メンバ関数
	CObjectX(int nPriority = 3);
	// 仮想関数
	~CObjectX();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	// 取得
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }

	// 静的メンバ関数
	static CObjectX* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	// 静的メンバ変数
	LPD3DXMESH m_pMesh;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアルへのポインタ
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	int* m_nIdxTex;				// テクスチャインデックスへのポインタ
};

#endif
