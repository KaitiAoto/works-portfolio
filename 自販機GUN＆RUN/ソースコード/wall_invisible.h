//==============================
//
// 透明な壁[wall_invisible.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _WALL_INVISIBLE_H_
#define _WALL_INVISIBLE_H_

// インクルード
#include "main.h"
#include "object3D.h"

// 透明な壁クラス
class CInvisibleWall:public CObject3D
{
public:

	// メンバ関数
	CInvisibleWall(int nPriority = 1);
	~CInvisibleWall();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize();
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	// 静的メンバ関数
	static CInvisibleWall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXCOLOR m_col;		// 色
	bool m_bUse;			// 使用しているか
	bool m_bDraw;			// 描画するか
	int m_nIdxTex;			// テクスチャのインデックス番号
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
};

#endif