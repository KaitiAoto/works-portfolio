//==============================================
//
// メッシュ円柱表示処理[meshcylinder.h]
// Author : Kaiti Aoto
//
//==============================================

// 二重インクルード防止
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

// インクルード
#include "main.h"
#include "object.h"

// マクロ定義
#define MESHCYLINDERVTX_X (8)	// 横
#define MESHCYLINDERVTX_Z (2)	// 縦
#define MAXCYLINDER_VTX ((MESHCYLINDERVTX_X + 1) * (MESHCYLINDERVTX_Z + 1))								// 頂点数
#define CYLINDERPOLYGON_NO (MESHCYLINDERVTX_X * MESHCYLINDERVTX_Z * 2 + (MESHCYLINDERVTX_Z - 1) * 4)	// ポリゴン数
#define CYLINDERINDEX_NO ((MESHCYLINDERVTX_X + 1) * 2 * MESHCYLINDERVTX_Z + (MESHCYLINDERVTX_Z - 1) * 2)// インデックス数
#define MESHCYLINDER_SIZE (150.0f)

// メッシュ円柱クラス
class CMeshCylinder :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_IN = 0,
		TYPE_OUT,
		TYPE_BOTHSIDES,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CMeshCylinder();
	~CMeshCylinder();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetHeight(float fHeight);

	// 静的メンバ関数
	static CMeshCylinder* Create(const char* pFilename,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type);

private:

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	TYPE m_type;						// 種類 
	bool m_bUse;						// 使用しているか
	int m_nIdxTex;						// テクスチャのインデックス番号
	float m_fHeight;
	float m_fWidth;
	D3DXCOLOR m_col;
};

#endif