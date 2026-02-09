//==============================
//
// オブジェクト2D[object2D.h]
// Author:kaiti
//
//==============================
#ifndef OBJECT2D_H_
#define OBJECT2D_H_

#include "main.h"
#include "object.h"

#define OBJECT2D_SIZE (50)

//オブジェクト2Dクラス
class CObject2D:public CObject
{
public:

	//メンバ関数
	//CObject2D();
	CObject2D(int nPriority = 6);
	~CObject2D();

	static CObject2D* Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	virtual void Draw(void);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	//取得処理
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//設定処理
	void SetBuff(LPDIRECT3DVERTEXBUFFER9 buff) { m_pVtxBuff = buff; }
	void SetTex(float U1, float U2, float V1, float V2);
	void SetColor(D3DCOLOR fColor);
	void SetSize(float fWidth, float fHeight);

	void SetIdxTex(const int nIdxTex) { m_nIdxTex = nIdxTex; }
private:
	
	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //頂点情報

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//角度

	float m_fLength; //対角線の長さ
	float m_fAngle;  //対角線の角度

	int m_nIdxTex;
};

#endif