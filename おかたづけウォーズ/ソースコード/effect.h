//===============================================
//
// エフェクト[effect.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _EFFECT_H_
#define _EFFECT_H_

// インクルード
#include "main.h"
#include "object.h"

// エフェクトクラス
class CEffect:public CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_THUNDER,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CEffect(int nPriority = static_cast<int>(Priority::Effect));
	~CEffect();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, TYPE type = TYPE_NONE);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fRadius, int nPriority = static_cast<int>(Priority::Effect), TYPE type = TYPE_NONE);

private:
	void SetSize(float fRadius);
	void SetColor(D3DXCOLOR col);

	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 角度
	D3DXVECTOR3 m_move;					// 移動量
	D3DXCOLOR m_col;					// 色
	float m_fLength;					// 対角線の長さ
	float m_fAngle;						// 対角線の角度
	float m_fRadius;					// 半径
	bool m_bUse;						// 使用しているか
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTex;						// テクスチャのインデックス番号
	int m_nLife;						// 寿命
};

#endif