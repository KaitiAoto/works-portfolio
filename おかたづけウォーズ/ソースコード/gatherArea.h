//===============================================
//
// 収集エリア[gatherArea.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GATHERAREA_H_
#define _GATHERAREA_H_

// インクルード
#include "main.h"
#include "meshcylinder.h"
#include "objectX.h"

// 雑貨をまとめるエリアクラス
class CGatherArea:public CMeshCylinder
{
public:

	// メンバ関数
	CGatherArea(int nPriority = static_cast<int>(Priority::Character));
	~CGatherArea();
	HRESULT Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fRadius, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetBlink(bool bBlink) { m_bBlink = bBlink; }
	void SetRising(bool bRising, float fRiseSpeed = 10.0f, float fMaxRise = 800.0f);
	// 静的メンバ関数
	static CGatherArea* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fRadius, float fHeight);
	// 取得
	static int GetAllIn(void) { return m_nAllIn; }
	static int GetDustIn(void) { return m_nDustIn; }
	static int GetGoodsIn(void) { return m_nGoodsIn; }

private:
	void Reaction(void);
	void Blink(void);
	void Rising(void);
	bool CollInteract(void);

	// 定数定義
	static constexpr float ROT_SPEED = 0.01f;
	static constexpr float RISE_SPEED = 1.0f;
	static constexpr float MAX_RISE = 50.0f;
	D3DXCOLOR COLOR = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.5f);

	// メンバ変数
	D3DXCOLOR m_col;		// エリアの色
	CObjectX* m_pObon;		// エリア用モデルへのポインタ
	bool m_bBlink;			// 点滅するか
	bool m_bRiseReaction;	// 上昇リアクションするか
	bool m_bRising;			// 上昇するか
	float m_fRotSpeed;		// 回転速度
	float m_fRiseSpeed;		// 上下速度
	float m_fMaxRise;		// 最大上昇量

	// 静的メンバ変数
	static int m_nAllIn;	// 範囲内の全オブジェクト数カウンター
	static int m_nDustIn;	// 範囲内のゴミオブジェクト数カウンター
	static int m_nGoodsIn;	// 範囲内の雑貨オブジェクト数カウンター
};

#endif