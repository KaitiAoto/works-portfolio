//=================================
//
// 円エフェクト[effect_circle.h]
// Author : Kaiti Aoto
//
//=================================

// 二重インクルード防止
#ifndef _EFFECT_CIRCLE_H_
#define _EFFECT_CIRCLE_H_

// インクルード
#include "main.h"
#include "object.h"
#include "meshcircle.h"

// 円エフェクトクラス
class CEffectCircle:public CObject
{
public:
	//種類
	typedef enum
	{
		TYPE_UPDOWN = 0,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CEffectCircle(int nPriority = 4);
	~CEffectCircle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, CMeshCircle::TYPE circle, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//設定処理

	// 静的メンバ関数
	static CEffectCircle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, CMeshCircle::TYPE circle, TYPE type);

private:

	void UpDown(void);
	
	//メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOff;	// 初期位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXCOLOR m_col;		// 色
	float m_fRadius;		// 半径
	int m_nLife;			// 寿命
	bool m_bUse;			// 使用しているか
	bool m_bUp;				// 上がるか下がるか
	TYPE m_type;			// 種類
	CMeshCircle* m_pCircle;	// 円メッシュへのポインタ
};

#endif