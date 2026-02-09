//================================
//
// モデル表示処理[objectX.cpp]
// Author:kaiti
//
//================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "texture.h"
//================
// コンストラクタ
//================
CObjectX::CObjectX(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pMesh = NULL; //メッシュ(頂点情報)へのポインタ
	m_pBuffMat = NULL;//マテリアルへのポインタ
	m_dwNumMat = 0;//マテリアルの数
	m_nIdxTex = 0;
}
//================
// デストラクタ
//================
CObjectX::~CObjectX()
{
}
//===========
// 生成処理
//===========
CObjectX* CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObjectX* pObjectX;
	pObjectX = new CObjectX;
	//初期化
	pObjectX->Init(pos, rot);
	return pObjectX;
}
//================
//モデルの初期化
//================
HRESULT CObjectX::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CTexture* pTex = CManager::GetTex();

	m_pos = pos;
	m_rot = rot;

	//Xファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX("data\\MODEL\\vending_machine00.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	if (FAILED(hr))
	{
		// エラー処理
		return hr;
	}

	//必要数のテクスチャへのIndexのポインタを動的確保
	m_nIdxTex = new int[m_dwNumMat];

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			int nIdx = pTex->Register(pMat[nCntMat].pTextureFilename);
			m_nIdxTex[nCntMat] = nIdx;  // インデックスを保存
		}
	}

	return S_OK;
}
//==================
//モデルの終了処理
//==================
void CObjectX::Uninit(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	if (m_nIdxTex != NULL)
	{
		delete[] m_nIdxTex;
		m_nIdxTex = nullptr;
	}
}
//=================
//モデルの更新処理
//=================
void CObjectX::Update(void)
{

}
//==============
//モデルの描画
//===============
void CObjectX::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//テクスチャ取得
	CTexture* pTex = CManager::GetTex();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	//マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクスチャの設定
		if (m_nIdxTex[nCntMat] >= 0)
		{
			pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex[nCntMat]));
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		//モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	pDevice->SetMaterial(&matDef);
}
//
//
//
void CObjectX::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
}