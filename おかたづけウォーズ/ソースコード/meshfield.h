//===============================================
//
// メッシュ床表示処理[meshfield.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

// インクルード
#include "main.h"
#include "object.h"

// メッシュ床クラス
class CMeshField:public CObject
{
public:

	// メンバ関数
	CMeshField();
	~CMeshField();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int vtx_x = 15, int vtx_z = 15);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CMeshField* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int vtx_x = 15, int vtx_z = 15);

private:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ	
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTex;						// テクスチャのインデックス番号

	int VTX_X;							// 横分割数
	int VTX_Z;							// 奥分割数
	int MAX_VTX;						// 頂点数
	int POLYGON_NO;						// ポリゴン数
	int INDEX_NO;						// インデックス数

};

#endif