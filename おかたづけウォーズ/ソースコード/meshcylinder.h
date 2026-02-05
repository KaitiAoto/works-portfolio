//===============================================
//
// メッシュ円柱表示処理[meshcylinder.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

// インクルード
#include "main.h"
#include "object.h"

// メッシュ円柱クラス
class CMeshCylinder :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_IN = 0,	// 内側
		TYPE_OUT,		// 外側
		TYPE_BOTHSIDES,	// 両面
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CMeshCylinder(int nPriority = static_cast<int>(Priority::StageModel));
	~CMeshCylinder();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x = 16, int vtx_y = 1);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col);
	void SetHeight(float fHeight);
	void SetUse(bool bUse) { m_bUse = bUse; }
	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetRadius(void) { return m_fRadius; }

	// 静的メンバ関数
	static CMeshCylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x = 16, int vtx_y = 1);

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
	float m_fHeight;					// 高さ
	float m_fRadius;					// 半径
	D3DXCOLOR m_col;					// 色

	int VTX_X;							// X軸分割数
	int VTX_Y;							// Y軸分割数
	int MAX_VTX;						// 頂点数
	int POLYGON_NO;						// ポリゴン数
	int INDEX_NO;						// インデックス数

};

#endif