//=====================
//
// 数字[number.h]
// Author : Kaiti Aoto
//
//=====================

// 二重インクルード防止
#ifndef _NUMBER_H_
#define _NUMBER_H_

// インクルード
#include "main.h"
#include "object.h"

// マクロ定義
#define NUMBER_SIZE (25)

// 数字クラス
class CNumber
{
public:

	// メンバ関数
	CNumber();
	~CNumber();

	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	LPDIRECT3DVERTEXBUFFER9 GetBuff(void) { return m_pVtxBuff; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// 設定処理
	void SetBuff(LPDIRECT3DVERTEXBUFFER9 buff) { m_pVtxBuff = buff; }
	void SetPos(D3DXVECTOR3 pos);
	void SetTex(float U1, float U2, float V1, float V2);
	void SetColor(D3DXCOLOR Color);
	void SetIdxTex(int nIdx) { m_nIdxTex = nIdx; }
	void SetSize(float fWidth, float fHeight);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	// 静的メンバ関数
	static CNumber* Create(const char* pFilename, D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点情報
	D3DXVECTOR3 m_pos;					// 位置
	D3DCOLOR m_col;
	D3DXVECTOR3 m_rot;					//角度
	float m_fLength;					//対角線の長さ
	float m_fAngle;						//対角線の角度
	float m_fWidth;						// 横幅
	float m_fHeight;					// 高さ
	int m_nIdxTex;						// テクスチャのインデックス番号
};

#endif