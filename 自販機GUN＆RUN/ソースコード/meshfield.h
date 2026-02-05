//=====================================
//
// メッシュ床表示処理[meshfield.h]
// Author : Kaiti Aoto
//
//=====================================

// 二重インクルード防止
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

// インクルード
#include "main.h"
#include "object.h"

// マクロ定義
#define MESHVTX_X (15)	// 横
#define MESHVTX_Z (15)	// 縦
#define MAX_VTX ((MESHVTX_X + 1) * (MESHVTX_Z + 1))						// 頂点数
#define POLYGON_NO (MESHVTX_X * MESHVTX_Z * 2 + (MESHVTX_Z - 1) * 4)	//	ポリゴン数
#define INDEX_NO ((MESHVTX_X + 1) * 2 * MESHVTX_Z + (MESHVTX_Z - 1) * 2)// インデックス数
#define MESH_SIZE (500.0f)

// メッシュ床クラス
class CMeshField:public CObject
{
public:

	// メンバ関数
	CMeshField();
	~CMeshField();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CMeshField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

private:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ	
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTex;						// テクスチャのインデックス番号

};

#endif