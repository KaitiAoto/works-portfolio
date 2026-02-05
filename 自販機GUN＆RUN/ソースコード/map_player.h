//====================================
//
// マッププレイヤー[map_player.h]
// Author : Kaiti Aoto
//
//====================================

// 二重インクルード防止
#ifndef _MAP_PLAYER_H_
#define _MAP_PLAYER_H_

// インクルード
#include "main.h"
#include "object2D.h"
#include "bullet.h"

// マッププレイヤークラス
class CMapPlayer:public CObject2D
{
public:

	// メンバ関数
	CMapPlayer(int nPriority = 9);
	~CMapPlayer();

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PlayerMove(void);
	void ArrowMove(void);

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	bool GetUse(void){return m_bUse;}
	//設定処理
	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CMapPlayer* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	
	//メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	int m_nIdxTex;		// テクスチャのインデックス番号
	bool m_bUse;		// 使用しているか
	CObject2D* m_pArrow;
};

#endif