//============================
//
// モデル表示処理[model.h]
// Author:kaiti
//
//============================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

class CMotion;

#define MAX_PARTS (13)//パーツの最大数
#define NUM_MODEL (13)

class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CModel* Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }

	void SetParent(CModel* pModel) { m_pParent = pModel; }
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_Offsetpos = pos, m_Offsetrot = rot; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	void SetOffPos(D3DXVECTOR3 pos) { m_Offsetpos = pos; };
	void SetOffRot(D3DXVECTOR3 rot) { m_Offsetrot = rot; };
	D3DXVECTOR3 SetSize(void);

	static void Load(const char* pFilename, CModel** model, int& outNumModel);

private:
	LPD3DXMESH m_pMesh;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアルへのポインタ
	DWORD m_dwNumMat;			//マテリアルの数

	D3DXVECTOR3 m_pos;			//位置(オフセット)
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_Offsetpos;	//位置(オフセット)
	D3DXVECTOR3 m_Offsetrot;	//向き

	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	int* m_nIdxTex;				//テクスチャインデックスへのポインタ
	CModel* m_pParent;
};

//void SkipCommentLine(FILE* pFile);

#endif
