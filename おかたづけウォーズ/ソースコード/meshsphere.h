//===============================================
//
// メッシュ球表示処理[meshsphere.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

// インクルード
#include "main.h"
#include "object.h"

// メッシュ球クラス
class CMeshSphere :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_IN = 0,		// 内側
		TYPE_OUT,			// 外側
		TYPE_BOTHSIDES,		// 両面
		TYPE_HALF_TOP,		// 上半分
		TYPE_HALF_BOTTOM,	// 下半分
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CMeshSphere();
	~CMeshSphere();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x = 20, int vtx_z = 20);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 設定
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CMeshSphere* Create(const char* pFilename,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x = 20, int vtx_z = 20);

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

	int VTX_X;							// 横分割数
	int VTX_Z;							// 縦分割数
	int MAX_VTX;						// 頂点数
	int STRIP_COUNT;					// インデックス数
	int STRIP_TOTAL;					//
	int INDEX_NO;						// 両面分含む最大数

};

#endif