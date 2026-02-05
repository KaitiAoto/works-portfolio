//==============================
//
// アイテム[item.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _ITEM_H_
#define _ITEM_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "player.h"

// マクロ定義
#define ITEM_LIFE (1200)
#define ITEM_HEAL (PLAYER_LIFE / 4)
#define ITEM_BULLET (10)
// アイテムクラス
class CItem:public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_BULLET = 0,
		TYPE_LIFE,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CItem(int nPriority = PRIORITY_ITEM);
	~CItem();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Bonus(void);

	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	TYPE GetType(void) { return m_type; }
	bool GetUse(void) { return m_bUse; }
	// 設定
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CItem* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);

private:
	void Move(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置(オフセット)
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rotDest;	// 目標の向き
	D3DXVECTOR3 m_size;		// サイズ
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス

	CModel* m_pModel;		// モデルへのポインタ
	int m_nLife;			// 寿命
	TYPE m_type;			// 種類
	bool m_bUse;			// 使用しているか
	float m_fFloatTime;		// 浮遊時間
};

#endif