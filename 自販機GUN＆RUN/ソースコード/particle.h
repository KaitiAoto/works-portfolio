//==============================
//
// パーティクル[particle.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

// インクルード
#include "main.h"
#include "object.h"
#include "effect.h"

// パーティクルクラス
class CParticle:public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_SMOKE,
		TYPE_SPRAY,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CParticle();
	~CParticle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	// 設定処理

	// 静的メンバ関数
	static CParticle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fRadius, TYPE type);

private:
	void SetElse(D3DXCOLOR col, int nLife, float fRadius);

	void None(void);
	void Smoke(void);
	void Spray(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 角度
	float m_fLength;	// 対角線の長さ
	float m_fAngle;		// 対角線の角度
	D3DXCOLOR m_col;	// 色
	float m_fRadius;	// 半径
	int m_nLife;		// 寿命
	bool m_bUse;		// 使用しているか
	TYPE m_type;		// 種類
};

#endif