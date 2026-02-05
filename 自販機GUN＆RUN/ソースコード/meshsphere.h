//==============================================
//
// メッシュ球表示処理[meshsphere.h]
// Author : Kaiti Aoto
//
//==============================================

// 二重インクルード防止
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

// インクルード
#include "main.h"
#include "object.h"

// マクロ定義
#define MESHSPHEREVTX_X (20)
#define MESHSPHEREVTX_Z (20)
#define MAXSPHERE_VTX ((MESHSPHEREVTX_X + 1) * (MESHSPHEREVTX_Z + 1))	// 頂点数
#define SPHERE_STRIP_COUNT ((MESHSPHEREVTX_X + 1) * 2 + 2)				// インデックス数
#define SPHERE_STRIP_TOTAL (MESHSPHEREVTX_Z)
#define SPHEREINDEX_NO (SPHERE_STRIP_COUNT * SPHERE_STRIP_TOTAL * 2)	// 両面分含む最大数
#define MESHSPHERE_SIZE (150.0f)

// メッシュ球クラス
class CMeshSphere :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_IN = 0,
		TYPE_OUT,
		TYPE_BOTHSIDES,
		TYPE_HALF_TOP,
		TYPE_HALF_BOTTOM,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CMeshSphere();
	~CMeshSphere();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 設定
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CMeshSphere* Create(const char* pFilename,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type);

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
};

#endif