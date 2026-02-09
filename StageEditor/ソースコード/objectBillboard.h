//=====================================
//
// ビルボード表示処理[billboard.h]
// Author:kaiti
//
//=====================================
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

#include "main.h"
#include "object.h"

#define BILLBOARD_SIZE (20)

//クラス
class CObjectBillboard:public CObject
{
public:
	//メンバ関数
	CObjectBillboard(int nPriority = 3);
	//仮想関数
	~CObjectBillboard();
	HRESULT Init(const char* pTexName,D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	static CObjectBillboard* Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius);

	void SetSize(float fRadius);
	void SetColor(D3DXCOLOR col);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;

	D3DXVECTOR3 m_pos;//位置
	D3DXVECTOR3 m_rot;//向き
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	int m_nIdxTex;
};

#endif
