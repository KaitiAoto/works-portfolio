//==============================
//
// テクスチャ[texture.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// インクルード
#include "main.h"

// マクロ定義
#define MAX_TEXTURE (128)

// テクスチャクラス
class CTexture
{
public:

	// メンバ関数
	CTexture();
	// 仮想関数
	~CTexture();
	HRESULT Load(void);
	void Unload(void);
	int Register(const char* pFilename);
	// 取得
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_apTex[MAX_TEXTURE];	// テクスチャ
	char m_apFileName[MAX_TEXTURE][MAX_PATH];	// テクスチャ名

	// 静的メンバ関数
	static int m_nNumAll;						// 最大数
};

#endif