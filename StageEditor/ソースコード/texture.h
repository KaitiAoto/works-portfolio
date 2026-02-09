//==============================
//
// テクスチャ[texture.h]
// Author:kaiti
//
//==============================
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "main.h"

#define MAX_TEXTURE (128)

//オブジェクトクラス
class CTexture
{
public:

	//メンバ関数
	CTexture();
	//仮想関数
	~CTexture();
	HRESULT Load(void);
	void Unload(void);
	int Register(const char* pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	const char* GetFilename(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_apTex[MAX_TEXTURE];
	char m_apFileName[MAX_TEXTURE][MAX_PATH];
	static int m_nNumAll;
};

#endif