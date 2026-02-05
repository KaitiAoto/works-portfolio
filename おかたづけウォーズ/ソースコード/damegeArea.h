//==============================================
//
// ダメージエリア処理[damegeArea.h]
// Author : Kaiti Aoto
//
//==============================================

// 二重インクルード防止
#ifndef _DAMEGEAREA_H_
#define _DAMEGEAREA_H_

// インクルード
#include "meshcircle.h"

// ダメージエリアクラス
class CDamegeArea :public CMeshCircle
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,	// 通常
		TYPE_FIRE,		// 火炎放射
		TYPE_THUNDER,	// 落雷
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CDamegeArea();
	~CDamegeArea();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CDamegeArea* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int nDamege, int nDamegeTime);

private:
	// 当たり判定
	bool CollPlayer(void);
	void CollInteract(void);
	void CollInteractFire(void);

	// 種類別の更新
	void UpdateNone(void);
	void UpdateFire(void);
	void UpdateThunder(void);

	// メンバ変数
	CMeshCircle*m_pMaxSize;	// 最大サイズ表示円へのポインタ
	bool m_bCntTime;		// ダメージを受けるまでの時間を計測するか
	float m_fNowRadius;		// 今の半径
	float m_fMaxRadius;		// 最大時の半径
	int m_nDamegeTime;		// ダメージを受けるまでの時間
	int m_nDamege;			// ダメージ
	int m_nCntFireTime;		// 火炎放射時間
	TYPE m_type;			// 種類
};

#endif