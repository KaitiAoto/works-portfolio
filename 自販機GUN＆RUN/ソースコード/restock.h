//==============================
//
// 補充処理[restock.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _RESTOCK_H_
#define _RESTOCK_H_

// インクルード
#include "main.h"
#include "object2D.h"
#include "bullet.h"
#include "vender.h"

// マクロ定義
#define RESTOCK_SIZE (250)

// オブジェクト2Dクラス
class CRestock:public CObject2D
{
public:

	// メンバ関数
	CRestock(int nPriority = 7);
	~CRestock();

	static CRestock* Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetUse(void){return m_bUse;}
	// 設定処理
	void Set(int nRestock, CBullet::TYPE type, CVender* pVender);
	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
	void SetUse(bool bUse) { m_bUse = bUse; }

private:
	
	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 角度

	int m_nIdxTex;		// テクスチャのインデックス番号
	bool m_bUse;		// 使用しているか
	bool m_bRLShake;
};

#endif