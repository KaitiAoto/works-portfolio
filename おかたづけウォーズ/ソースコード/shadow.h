//===============================================
//
// 影[shadow.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _SHADOW_H_
#define _SHADOW_H_

// インクルード
#include "main.h"
#include "object3D.h"

// 影クラス
class CShadow:public CObject3D
{
public:
	// 種類
	typedef enum
	{
		TYPE_CIRCLE = 0,	// 円
		TYPE_SQUARE,		// 四角
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CShadow(int nPriority = static_cast<int>(Priority::Field));
	~CShadow();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fX, float fZ);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CShadow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fX, float fZ,TYPE type = TYPE_CIRCLE);
private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXCOLOR m_col;		// 色
	TYPE m_type;			// 種類
	bool m_bUse;			// 使用しているか
	float m_fLength;		// 対角線の長さ
	float m_fRadius;		// 半径
	int m_nIdxTex;			// テクスチャのインデックス番号
};

#endif