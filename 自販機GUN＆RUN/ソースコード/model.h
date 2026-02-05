//============================
//
// モデル表示処理[model.h]
// Author : Kaiti Aoto
//
//============================

// 二重インクルード防止
#ifndef _MODEL_H_
#define _MODEL_H_

// インクルード
#include "main.h"

// 前方宣言
class CMotion;

// マクロ定義
#define MAX_PARTS (13)	// パーツの最大数
#define NUM_MODEL (13)

// モデルクラス
class CModel
{
public:
	// メンバ関数
	CModel();
	~CModel();

	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetScale(void) { return m_scale; }
	// 設定
	void SetParent(CModel* pModel) { m_pParent = pModel; }
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	void SetOffPos(D3DXVECTOR3 pos) { m_Offsetpos = pos; };
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };
	void SetOffRot(D3DXVECTOR3 rot) { m_Offsetrot = rot; };
	void SetOffSet(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_Offsetpos = pos, m_Offsetrot = rot; }
	void SetColorChange(bool bUse) { m_bColorChange = bUse; }
	void SetColor(D3DXCOLOR col) { m_forcedColor = col; }
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	D3DXVECTOR3 SetSize(void);

	// 静的メンバ関数
	static CModel* Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void Load(const char* pFilename, CModel** model, int& outNumModel);

private:
	// メンバ変数
	LPD3DXMESH m_pMesh;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアルへのポインタ
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_Offsetpos;	// 位置(オフセット)
	D3DXVECTOR3 m_Offsetrot;	// 向き(オフセット)
	D3DXVECTOR3 m_scale;		// スケール
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR m_forcedColor;	// 色
	CModel* m_pParent;			// 親モデル
	int* m_nIdxTex;				// テクスチャインデックスへのポインタ
	bool m_bColorChange;		// 色を変えるかどうか
};

#endif
