//===============================================
//
// ビルボード表示処理[billboard.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

// インクルード
#include "main.h"
#include "object.h"


// ビルボードクラス
class CObjectBillboard:public CObject
{
public:
	// メンバ関数
	CObjectBillboard(int nPriority = static_cast<int>(Priority::StageModel));
	// 仮想関数
	~CObjectBillboard();
	HRESULT Init(const char* pTexName,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	virtual void Draw(void);

	// 設定
	void SetSize(float fRadius);
	void SetColor(D3DXCOLOR col);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	// 静的メンバ関数
	static CObjectBillboard* Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPriority = static_cast<int>(Priority::StageModel));
	
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTex;						// テクスチャのインデックス番号
	bool m_bDraw;						// 描画するか
};

#endif
