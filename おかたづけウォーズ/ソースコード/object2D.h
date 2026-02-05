//===============================================
//
// オブジェクト2D[object2D.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

// インクルード
#include "main.h"
#include "object.h"

// オブジェクト2Dクラス
class CObject2D:public CObject
{
public:

	// メンバ関数
	CObject2D(int nPriority = static_cast<int>(Priority::UI));
	~CObject2D();
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	virtual void Draw(void);

	// 取得処理
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR2 GetSize(void) { return m_size; }

	// 設定処理
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; Set(m_pos, m_rot);}
	void SetBuff(LPDIRECT3DVERTEXBUFFER9 buff) { m_pVtxBuff = buff; }
	void SetTex(float U1, float U2, float V1, float V2);
	void SetColor(D3DCOLOR fColor);
	void SetSize(D3DXVECTOR2 size);
	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CObject2D* Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nPriority = static_cast<int>(Priority::UI));

private:
	
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点情報
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 角度
	D3DXVECTOR2 m_size;					// サイズ
	float m_fLength;					// 対角線の長さ
	float m_fAngle;						// 対角線の角度
	int m_nIdxTex;						// テクスチャのインデックス番号
	bool m_bUse;						// 使用しているか
};

#endif