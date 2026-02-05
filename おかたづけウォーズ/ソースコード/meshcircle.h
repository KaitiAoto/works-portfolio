//===============================================
//
// メッシュ円表示処理[meshcircle.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _MESHCIRCLE_H_
#define _MESHCIRCLE_H_

// インクルード
#include "main.h"
#include "object.h"

// メッシュ円クラス
class CMeshCircle :public CObject
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
	CMeshCircle(int nPriority = static_cast<int>(Priority::Effect));
	~CMeshCircle();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int vtx_x = 16, int vtx_z = 1);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetRadius(void) { return m_fRadius; }
	// 設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col);
	void SetSize(float fRadius);
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CMeshCircle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int vtx_x = 16, int vtx_z = 1);

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
	float m_fRadius;					// 半径

	int VTX_X;							// X軸分割数
	int VTX_Z;							// Z軸分割数
	int MAX_VTX;						// 頂点数
	int POLYGON_NO;						// ポリゴン数
	int INDEX_NO;						// インデックス数
};

#endif