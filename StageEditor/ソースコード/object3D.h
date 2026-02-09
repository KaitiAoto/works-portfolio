//=================================
//
// ポリゴン表示処理[object3D.h]
// Author:kaiti
//
//=================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

#define OBJECT3D_SIZE (500)

//クラス
class CObject3D:public CObject
{
public:
	typedef enum
	{
		TYPE_FIELD = 0,
		TYPE_WALL,
		TYPE_SHADOW,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CObject3D(int nPriority = 1);
	//仮想関数
	~CObject3D();
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CObject3D* Create(const char* pTexName,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type);
	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
	float GetHeight(D3DXVECTOR3 pos);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void Set(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	void SetColor(D3DXCOLOR col);

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	TYPE GetType(void) { return m_type; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	int GetTexIdx(void) { return m_nIdxTex; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;

	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス

	int m_nIdxTex;

	float m_fWidth;
	float m_fHeight;
	TYPE m_type;

	bool m_bDraw;
};

#endif
