//==============================
//
// 敵拠点[enemy_base.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _RIVAL_SHOP_H_
#define _RIVAL_SHOP_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "enemy_manager.h"
#include "meshcylinder.h"
#include "gauge_enemy.h"
#include "map_enemybase.h"
#include "gauge_enemybase.h"

// マクロ定義

// 敵拠点クラス
class CRivalShop:public CObject
{
public:
	// 状態
	typedef enum
	{
		STATE_NONE = 0,
		STATE_THRIVING, //繁盛してる
		STATE_STUGGLING,//繁盛してない
		STATE_MAX
	}STATE;

	// メンバ関数
	CRivalShop(int nPriority = PRIORITY_ENEMYBASE);
	~CRivalShop();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	// 取得
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetUse(void) { return m_bUse; }

	// 静的メンバ関数
	static CRivalShop* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
private:
	void Scale(void);
	void SetState(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_size;						// サイズ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	CModel* m_pModel;						// 通常時のモデルへのポインタ
	bool m_bUse;							// 使用しているか
	STATE m_state;
	// 静的メンバ変数

};

#endif