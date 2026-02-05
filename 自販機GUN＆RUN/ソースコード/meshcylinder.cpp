//==============================================
//
// メッシュ円柱表示処理[MeshCylinder.cpp]
// Author : Kaiti Aoto
//
//==============================================
#include "meshcylinder.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//==================
// コンストラクタ
//==================
CMeshCylinder::CMeshCylinder()
{
	m_pVtxBuff = nullptr;
	m_nIdxTex = 0;
	m_pIdxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_type = TYPE_IN;
	m_bUse = true;
}
//================
// デストラクタ
//================
CMeshCylinder::~CMeshCylinder()
{

}
//============
// 生成処理
//============
CMeshCylinder* CMeshCylinder::Create(const char* pFilename,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type)
{
	CMeshCylinder* pMesh;
	pMesh = new CMeshCylinder;
	//初期化
	pMesh->Init(pos, rot, fWidth, fHeight, col, type);

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	pMesh->m_nIdxTex = pTex->Register(pFilename);

	return pMesh;
}
//==================
// 初期化処理
//==================
HRESULT CMeshCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, D3DXCOLOR col, TYPE type)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_rot = rot;
	m_type = type;
	m_bUse = true;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_col = col;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAXCYLINDER_VTX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);

	int stripCount = (MESHCYLINDERVTX_X + 1) * 2 + 2; // 1ストリップのインデックス数
	int stripTotal = MESHCYLINDERVTX_Z;              // ストリップ数（Z方向）
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
	float fAngle = (D3DX_PI * 2.0f) / MESHCYLINDERVTX_X;

	for (int nCntV = 0; nCntV < MESHCYLINDERVTX_Z + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < MESHCYLINDERVTX_X + 1; nCntH++)
		{
			//角度
			float Angle = fAngle * nCntH;

			//頂点座標の設定
			pVtx[index].pos = D3DXVECTOR3(sinf(Angle) * fWidth, fHeight *(MESHCYLINDERVTX_Z - nCntV), cosf(Angle) * fWidth);
			
			D3DXVECTOR3 nor;//位置
			nor = pVtx[index].pos - m_pos;

			//法線ベクトル
			D3DXVec3Normalize(&pVtx[index].nor,&nor);

			//頂点カラーの設定
			pVtx[index].col = col;

			////テクスチャ座標の設定
			//pVtx[index].tex = D3DXVECTOR2((1.0f / MESHCYLINDERVTX_X) * nCntH, (1.0f / MESHCYLINDERVTX_Z) * nCntV);
			pVtx[index].tex = D3DXVECTOR2((float)nCntH, (float)nCntV / 2);

			index++;
		}
	}

	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	
	int nCntX;
	int idx = 0;
	for (int nCntZ = 0; nCntZ < MESHCYLINDERVTX_Z; nCntZ++)
	{
		for (nCntX = 0; nCntX <= MESHCYLINDERVTX_X; nCntX++)
		{
			int i0 = nCntX + (nCntZ * (MESHCYLINDERVTX_X + 1));
			int i1 = nCntX + ((nCntZ + 1) * (MESHCYLINDERVTX_X + 1));

			if (type == TYPE_IN)
			{
				pIdx[idx++] = (WORD)i1;
				pIdx[idx++] = (WORD)i0;
			}
			else if (type == TYPE_OUT)
			{
				pIdx[idx++] = (WORD)i0;
				pIdx[idx++] = (WORD)i1;
			}
			else if (type == TYPE_BOTHSIDES)
			{
				// 外向き
				pIdx[idx++] = (WORD)i0;
				pIdx[idx++] = (WORD)i1;
			}
		}

		// ディグナルトライアングル（デガレポリゴン）でストリップを区切る
		if (nCntZ < MESHCYLINDERVTX_Z - 1)
		{
			int i = (nCntX - 1) + (nCntZ * (MESHCYLINDERVTX_X + 1));
			pIdx[idx++] = (WORD)i;
			pIdx[idx++] = (WORD)i;
		}
	}

	// 両面描画の裏面分を追加（順番逆でインデックス再構築）
	if (type == TYPE_BOTHSIDES)
	{
		for (int nCntZ = 0; nCntZ < MESHCYLINDERVTX_Z; nCntZ++)
		{
			for (nCntX = 0; nCntX <= MESHCYLINDERVTX_X; nCntX++)
			{
				int i0 = nCntX + (nCntZ * (MESHCYLINDERVTX_X + 1));
				int i1 = nCntX + ((nCntZ + 1) * (MESHCYLINDERVTX_X + 1));

				// 内向き（逆順）
				pIdx[idx++] = (WORD)i1;
				pIdx[idx++] = (WORD)i0;
			}

			if (nCntZ < MESHCYLINDERVTX_Z - 1)
			{
				int i = (nCntX - 1) + (nCntZ * (MESHCYLINDERVTX_X + 1));
				pIdx[idx++] = (WORD)i;
				pIdx[idx++] = (WORD)i;
			}
		}
	}

	//インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//===============================
// メッシュ円柱の終了処理
//===============================
void CMeshCylinder::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	CObject::Release();
}
//===============================
// メッシュ円柱の更新処理
//===============================
void CMeshCylinder::Update(void)
{

}
void CMeshCylinder::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//円柱
	int index = 0;
	float fAngle = (D3DX_PI * 2.0f) / MESHCYLINDERVTX_X;

	for (int nCntV = 0; nCntV < MESHCYLINDERVTX_Z + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < MESHCYLINDERVTX_X + 1; nCntH++)
		{
			//角度
			float Angle = fAngle * nCntH;

			//頂点座標の設定
			pVtx[index].pos = D3DXVECTOR3(sinf(Angle) * m_fWidth, m_fHeight * (MESHCYLINDERVTX_Z - nCntV), cosf(Angle) * m_fWidth);

			D3DXVECTOR3 nor;//位置
			nor = pVtx[index].pos - m_pos;

			//法線ベクトル
			D3DXVec3Normalize(&pVtx[index].nor, &nor);

			//頂点カラーの設定
			pVtx[index].col = m_col;

			////テクスチャ座標の設定
			//pVtx[index].tex = D3DXVECTOR2((1.0f / MESHCYLINDERVTX_X) * nCntH, (1.0f / MESHCYLINDERVTX_Z) * nCntV);
			pVtx[index].tex = D3DXVECTOR2((float)nCntH, (float)nCntV / 2);

			index++;
		}
	}

	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();
}
//===============================
// メッシュ円柱の描画処理
//===============================
void CMeshCylinder::Draw(void)
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
		//ALPHAテストの設定
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MAXCYLINDER_VTX, 0, CYLINDERPOLYGON_NO);

		// 描画後、元のカリング設定に戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, oldCullMode);

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		//ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}