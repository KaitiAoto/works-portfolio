//==============================
//
// テクスチャ[texture.cpp]
// Author:kaiti
//
//==============================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
//静的メンバ変数
int CTexture::m_nNumAll = 0;
//==================
// コンストラクタ
//==================
CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_apTex[nCnt] = NULL;
		m_apFileName[nCnt][0] = NULL;
	}
}
//================
// デストラクタ
//================
CTexture::~CTexture()
{
}
//=======================
// 全テクスチャ読み込み
//=======================
HRESULT CTexture::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//
	const char* apFileName[MAX_TEXTURE] = 
	{
		"data\\TEXTURE\\build007.png",
		"data\\TEXTURE\\build008.png",
		
		"data\\TEXTURE\\bullet00.png",
		"data\\TEXTURE\\bullet01.png",
		"data\\TEXTURE\\bullet02.png",
		"data\\TEXTURE\\bullet03.png",
		"data\\TEXTURE\\bullet04.png",
		"data\\TEXTURE\\bullet05.png",
		"data\\TEXTURE\\bullet06.png",

		"data\\TEXTURE\\can00.jpg",	
		
		"data\\TEXTURE\\conveni00.png",
		"data\\TEXTURE\\conveni01.jpg",
		"data\\TEXTURE\\conveni02.jpg",	
		
		"data\\TEXTURE\\dust00.png",
		
		"data\\TEXTURE\\effect000.png",
		
		"data\\TEXTURE\\gauge00.jpeg",

		"data\\TEXTURE\\ice00.jpg",
		"data\\TEXTURE\\ice01.jpg",

		"data\\TEXTURE\\isi.jpg",
		
		"data\\TEXTURE\\money00.jpg",
		"data\\TEXTURE\\money01.png",
		
		"data\\TEXTURE\\number000.png",
		"data\\TEXTURE\\number003.png",
		"data\\TEXTURE\\number004.png",

		"data\\TEXTURE\\restock00.png",
		
		"data\\TEXTURE\\shadow000.jpg",

		"data\\TEXTURE\\sky001.jpg",

		"data\\TEXTURE\\vending01.jpg",

		"data\\TEXTURE\\vending03.jpg",

		"data\\TEXTURE\\vending04.jpg",
		"data\\TEXTURE\\vending04_1.jpg",

		"data\\TEXTURE\\vending05.jpg",
		
		"data\\TEXTURE\\vending06.jpg",

		"data\\TEXTURE\\field00.jpeg",

	};
	
	//テクスチャの読み込み
	for (int nCnt = 0; apFileName[nCnt] != nullptr && nCnt < MAX_TEXTURE; nCnt++)
	{
		HRESULT hr = D3DXCreateTextureFromFile(pDevice,
			apFileName[nCnt],
			&m_apTex[nCnt]);

		if (FAILED(hr))
		{//エラーチェック
			return hr;
		}
		else
		{
			//テクスチャ名保存
			strcpy(m_apFileName[nCnt], apFileName[nCnt]);

			m_nNumAll++;//総数加算
		}
	}

	return S_OK;
}
//=======================
// 全テクスチャ破棄
//=======================
void CTexture::Unload(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_apTex[nCnt] != NULL)
		{
			m_apTex[nCnt]->Release();
			m_apTex[nCnt] = NULL;
		}
	}
}
//=======================
// テクスチャ指定
//=======================
int CTexture::Register(const char* pFilename)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pFilename == NULL)
	{
		return -1;
	}

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (strcmp(pFilename, m_apFileName[nCnt]) == 0)
		{
			return nCnt;//Index番号を返す
		}
	}

	// 未登録の場合、新しく読み込む
	if (m_nNumAll < MAX_TEXTURE)
	{
		HRESULT hr = D3DXCreateTextureFromFile(pDevice, pFilename, &m_apTex[m_nNumAll]);
		if (SUCCEEDED(hr))
		{
			strcpy(m_apFileName[m_nNumAll], pFilename);
			return m_nNumAll++;
		}
		else
		{//エラーチェック
			char buf[256];
			sprintf(buf, "テクスチャ読み込み失敗: %s\n", pFilename);
			OutputDebugStringA(buf);
		}
	}

	return -1;
}
//==========================
// テクスチャアドレス取得
//==========================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx < 0 || nIdx >= MAX_TEXTURE)
	{//エラーチェック
		OutputDebugStringA("不正なテクスチャインデックス\n");
		return NULL;
	}
	return m_apTex[nIdx];
}

const char* CTexture::GetFilename(int nIdx)
{
	if (nIdx <= 0 || nIdx >= MAX_TEXTURE)
		return nullptr;
	return m_apFileName[nIdx];
}