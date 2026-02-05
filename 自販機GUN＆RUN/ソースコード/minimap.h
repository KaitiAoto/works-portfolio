//==============================
//
// ミニマップ[minimap.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

// インクルード
#include "object.h"
#include "map_player.h"

// マップクラス
class CMap:public CObject
{
public:
	// メンバ関数
	CMap();
	~CMap();

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	// 静的メンバ関数
	static CMap* Create(const D3DXVECTOR3 pos, float fWidth, float fHeight);
	// 取得
	static CMapPlayer* GetMapPlayer(void) { return m_pMapPlayer; }

private:
	//メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ

	// 静的メンバ変数
	static CMapPlayer* m_pMapPlayer;	//マッププレイヤーへのポインタ 
};

#endif