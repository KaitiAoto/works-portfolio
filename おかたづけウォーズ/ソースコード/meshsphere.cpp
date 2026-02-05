//===============================================
//
// メッシュ球表示処理[MeshSphere.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "meshsphere.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CMeshSphere::CMeshSphere()
{
	m_pVtxBuff = nullptr;
	m_nIdxTex = 0;
	m_pIdxBuff = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
	m_type = TYPE_IN;
	m_bUse = true;
}
//===============================================
// デストラクタ
//===============================================
CMeshSphere::~CMeshSphere()
{

}
//===============================================
// 生成処理
//===============================================
CMeshSphere* CMeshSphere::Create(const char* pFilename,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x, int vtx_z)
{
	CMeshSphere* pMesh;
	pMesh = new CMeshSphere;

	//初期化
	pMesh->Init(pos, rot, fWidth, fHeight, col, type, vtx_x, vtx_z);

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	pMesh->m_nIdxTex = pTex->Register(pFilename);

	return pMesh;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CMeshSphere::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type, int vtx_x, int vtx_z)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// メッシュ情報設定
	VTX_X = vtx_x;	// 横分割数
	VTX_Z = vtx_z;	// 縦分割数
	MAX_VTX = ((VTX_X + 1) * (VTX_Z + 1));		// 頂点数
	STRIP_COUNT = (VTX_X + 1) * 2 + 2;			// インデックス数
	STRIP_TOTAL = VTX_Z;
	INDEX_NO = STRIP_COUNT * STRIP_TOTAL * 2;	// 両面分含む最大数

	m_pos = pos;
	m_rot = rot;
	m_type = type;
	m_bUse = true;

	// 緯度範囲（半球用）
	int nLatStart = 0;
	 int nLatEnd = VTX_Z;
	if (type == TYPE_HALF_TOP) 
	{
		nLatEnd = VTX_Z / 2;
	}
	else if (type == TYPE_HALF_BOTTOM) 
	{
		nLatStart = VTX_Z / 2;
	}
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VTX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);

	int stripCount = (VTX_X + 1) * 2 + 2; // 1ストリップのインデックス数
	int stripTotal = VTX_Z;              // ストリップ数（Z方向）
	int nIndexCount = stripCount * stripTotal;

	if (type == TYPE_BOTHSIDES)
	{
		nIndexCount *= 2; // 両面分
	}
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * nIndexCount,
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX16,
							   D3DPOOL_MANAGED,
							   &m_pIdxBuff,
							   NULL);

	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//円柱
	int index = 0;
	for (int lat = nLatStart; lat <= nLatEnd; lat++) 
	{
		float theta = D3DX_PI * ((float)lat / VTX_Z);
		float y = cosf(theta);
		float r = sinf(theta);
		for (int lon = 0; lon <= VTX_X; lon++) 
		{
			float phi = D3DX_PI * 2.0f * lon / VTX_X;
			float x = r * cosf(phi);
			float z = r * sinf(phi);
			D3DXVECTOR3 posSphere = D3DXVECTOR3(x * fWidth, y * fHeight, z * fWidth);
			pVtx[index].pos = posSphere;
			D3DXVec3Normalize(&pVtx[index].nor, &posSphere);
			pVtx[index].col = col;
			pVtx[index].tex = D3DXVECTOR2((float)lon / VTX_X, (float)(lat - nLatStart) / (nLatEnd - nLatStart));
			index++;
		}
	}
	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	
	int idx = 0;
	for (int z = 0; z < (nLatEnd - nLatStart); z++) 
	{
		for (int x = 0; x <= VTX_X; x++) 
		{
			int i0 = x + (z * (VTX_X + 1));
			int i1 = x + ((z + 1) * (VTX_X + 1));
			if (type == TYPE_IN) 
			{
				pIdx[idx++] = (WORD)i0; pIdx[idx++] = (WORD)i1;
			}
			else if (type == TYPE_OUT) 
			{
				pIdx[idx++] = (WORD)i1; pIdx[idx++] = (WORD)i0;
			}
			else 			
			{
				pIdx[idx++] = (WORD)i0; pIdx[idx++] = (WORD)i1;
			}
		}
		if (z < (nLatEnd - nLatStart) - 1) 
		{
			int i = VTX_X + (z * (VTX_X + 1));
			pIdx[idx++] = (WORD)i;
			pIdx[idx++] = (WORD)i;
		}
	}
	if (type == TYPE_BOTHSIDES) 
	{
		for (int z = 0; z < (nLatEnd - nLatStart); z++) 
		{
			for (int x = 0; x <= VTX_X; x++) 
			{
				int i0 = x + (z * (VTX_X + 1));
				int i1 = x + ((z + 1) * (VTX_X + 1));
				pIdx[idx++] = (WORD)i1; pIdx[idx++] = (WORD)i0;
			}
			if (z < (nLatEnd - nLatStart) - 1) 
			{
				int i = VTX_X + (z * (VTX_X + 1));
				pIdx[idx++] = (WORD)i; pIdx[idx++] = (WORD)i;
			}
		}
	}
	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//===============================================
// メッシュ円柱の終了処理
//===============================================
void CMeshSphere::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	CObject::Release();
}
//===============================================
// メッシュ円柱の更新処理
//===============================================
void CMeshSphere::Update(void)
{

}
//===============================================
// メッシュ円柱の描画処理
//===============================================
void CMeshSphere::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	if (m_bUse == true)
	{
		//ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(m_pIdxBuff);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex));

		DWORD oldCullMode;
		// 現在のカリング設定を保存
		pDevice->GetRenderState(D3DRS_CULLMODE, &oldCullMode);

		// タイプに応じてカリング設定を切り替え
		switch (m_type)
		{
		case TYPE_OUT:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			break;

		case TYPE_IN:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			break;

		case TYPE_BOTHSIDES:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			break;
		}
		//メッシュ円柱を描画
		for (int i = 0; i < VTX_Z; ++i)
		{
			int startIndex = i * ((VTX_X + 1) * 2 + 2);
			int primitiveCount = (VTX_X * 2 + 2) - 2;

			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAX_VTX, startIndex, primitiveCount);
		}

		// 描画後、元のカリング設定に戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, oldCullMode);

		//ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}