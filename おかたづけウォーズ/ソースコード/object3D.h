//===============================================
//
// ポリゴン表示処理[object3D.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

// インクルード
#include "main.h"
#include "object.h"

// オブジェクト3Dクラス
class CObject3D:public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_FIELD= 0,	// フィールド
		TYPE_WALL,		// 壁
		TYPE_SHADOW,	// 影
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CObject3D(int nPriority = static_cast<int>(Priority::Map));
	// 仮想関数
	~CObject3D();
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetColor(D3DXCOLOR col);
	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	
	// 静的メンバ関数
	static CObject3D* Create(const char* pTexName,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type);

private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バッファ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nIdxTex;						// テクスチャのインデックス番号
	float m_fHeight;
	float m_fWidth;
};

#endif
