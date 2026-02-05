//==============================
//
// ステンシル影[shadowS.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _SHADOWS_H_
#define _SHADOWS_H_

// インクルード
#include "main.h"
#include "objectX.h"
#include "object2D.h"

// ステンシル影クラス
class CShadowS:public CObjectX
{
public:

	// メンバ関数
	CShadowS(int nPriority = 5);
	~CShadowS();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_pos = pos; m_rot = rot; }

	// 静的メンバ関数
	static CShadowS* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXCOLOR m_col;		// 色
	bool m_bUse;			// 使用しているか
	float m_fLength;		// 対角線の長さ
	float m_fRadius;		// 半径
	int m_nIdxTex;			// テクスチャのインデックス番号
	CObject2D* m_pObj2D;	// 2Dオブジェクトへのポインタ
};

#endif