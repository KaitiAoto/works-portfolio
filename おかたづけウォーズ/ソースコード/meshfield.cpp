//===============================================
//
// メッシュ床表示処理[meshfield.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CMeshField::CMeshField()
{
	// 値をクリア
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_nIdxTex = 0;
}
//===============================================
// デストラクタ
//===============================================
CMeshField::~CMeshField()
{
}
//===============================================
// 生成処理
//===============================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int vtx_x, int vtx_z)
{
	// インスタンス生成
	CMeshField* pMesh = new CMeshField;

	// 初期化
	pMesh->Init(pos, rot, fWidth, fHeight, vtx_x, vtx_z);

	return pMesh;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int vtx_x, int vtx_z)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// メッシュ情報設定
	VTX_X = vtx_x;	// 横分割数
	VTX_Z = vtx_z;	// 縦分割数
	MAX_VTX = ((VTX_X + 1) * (VTX_Z + 1));					// 頂点数
	POLYGON_NO = (VTX_X * VTX_Z * 2 + (VTX_Z - 1) * 4);		// ポリゴン数
	INDEX_NO = ((VTX_X + 1) * 2 * VTX_Z + (VTX_Z - 1) * 2);	// インデックス数

	m_pos = pos;
	m_rot = rot;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VTX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * INDEX_NO,
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX16,
							   D3DPOOL_MANAGED,
							   &m_pIdxBuff,
							   NULL);

	VERTEX_3D* pVtx = NULL;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int n = 0;
	for (int nCntZ = 0; nCntZ <= VTX_Z; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= VTX_X; nCntX++)
		{
			// 頂点座標の設定
			float cellWidth = (fWidth * 2.0f) / VTX_X;
			float cellHeight = (fHeight * 2.0f) / VTX_Z;

			pVtx[n].pos = D3DXVECTOR3(-fWidth + (cellWidth * nCntX), 0.0f, fHeight - (cellHeight * nCntZ));
			// 法線ベクトルの設定
			pVtx[n].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			// 頂点カラーの設定
			pVtx[n].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ座標の設定
			pVtx[n].tex = D3DXVECTOR2((1.0f / VTX_X) * nCntX, (1.0f / VTX_Z) * nCntZ);

			n++;
		}
	}

	// 頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	// インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	
	int index = 0;
	for (int z = 0; z < VTX_Z; ++z)
	{
		if (z > 0)
		{
			// デジェネレート（最初の2つ）
			pIdx[index++] = (WORD)(z * (VTX_X + 1));
			pIdx[index++] = (WORD)(z * (VTX_X + 1));
		}

		for (int x = 0; x <= VTX_X; ++x)
		{
			pIdx[index++] = (WORD)((z + 1) * (VTX_X + 1) + x);
			pIdx[index++] = (WORD)(z * (VTX_X + 1) + x);
		}
	}
	m_pIdxBuff->Unlock();

	SetObjType(TYPE_MESH);	// オブジェクトのタイプ

	return S_OK;
}
//===============================================
// メッシュ床の終了処理
//===============================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	// オブジェクト破棄
	CObject::Release();
}
//===============================================
// メッシュ床の更新処理
//===============================================
void CMeshField::Update(void)
{

}
//===============================================
// メッシュ床の描画処理
//===============================================
void CMeshField::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex));

	// メッシュ床を描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VTX, 0, POLYGON_NO);
}