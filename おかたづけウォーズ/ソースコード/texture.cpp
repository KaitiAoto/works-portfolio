//===============================================
//
// テクスチャ[texture.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "texture.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数
int CTexture::m_nNumAll = 0;

//===============================================
// コンストラクタ
//===============================================
CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_apTex[nCnt] = nullptr;
		m_apFileName[nCnt][0] = NULL;
	}
}
//===============================================
// デストラクタ
//===============================================
CTexture::~CTexture()
{
}
//===============================================
// 全テクスチャ読み込み
//===============================================
HRESULT CTexture::Load(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//
	const char* apFileName[MAX_TEXTURE] = 
	{

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
//===============================================
// 全テクスチャ破棄
//===============================================
void CTexture::Unload(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_apTex[nCnt] != nullptr)
		{
			m_apTex[nCnt]->Release();
			m_apTex[nCnt] = nullptr;
		}
	}
}
//===============================================
// テクスチャ指定
//===============================================
int CTexture::Register(const char* pFilename)
{
	if (this == nullptr) return 0;
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pFilename == nullptr)
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
//===============================================
// テクスチャアドレス取得
//===============================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (this == nullptr) return 0;

	if (nIdx < 0 || nIdx >= MAX_TEXTURE)
	{//エラーチェック
		//OutputDebugStringA("不正なテクスチャインデックス\n");
		return nullptr;
	}
	return m_apTex[nIdx];
}