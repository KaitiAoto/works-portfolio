//==============================================
//
// メッシュ壁表示処理[meshWall.cpp]
// Author:kaiti
//
//==============================================
#include "meshwall.h"
//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;
//インデックスバッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;

MeshWall g_MeshWall[MAX_MESHWALL];
//=================================
// メッシュ壁の初期化処理
//=================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_MeshWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].bUse = false;
	}
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\kabe.jpg", &g_pTextureMeshWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * WALLMAX_VTX * MAX_MESHWALL,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffMeshWall,
		                        NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * WALLINDEX_NO * MAX_MESHWALL,
							   D3DUSAGE_WRITEONLY,
							   D3DFMT_INDEX16,
							   D3DPOOL_MANAGED,
							   &g_pIdxBuffMeshWall,
							   NULL);

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		VERTEX_3D* pVtx = NULL;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ <= WALLMESHVTX_Y; nCntZ++)
		{
			for (int nCntX = 0; nCntX <= WALLMESHVTX_X; nCntX++)
			{
				//頂点座標の設定
				//pVtx[0].pos = D3DXVECTOR3(-WALLMESH_SIZE + (WALLMESH_SIZE * nCntX), 0.0f, WALLMESH_SIZE - (WALLMESH_SIZE * nCntZ));
				pVtx[0].pos = D3DXVECTOR3(-WALLMESH_SIZE + (WALLMESH_SIZE * nCntX), WALLMESH_SIZE - (WALLMESH_SIZE * nCntZ), g_MeshWall[nCnt].pos.z);

				//法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((2.0f / WALLMESHVTX_X) * nCntX, (2.0f / WALLMESHVTX_Y) * nCntZ);

				pVtx++;
			}
		}
		//頂点バッファをアンロック　
		g_pVtxBuffMeshWall->Unlock();

		//インデックスバッファをロック
		WORD* pIdx = NULL;
		g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

		int nCntX, nCntZ;
		for (nCntZ = 0; nCntZ < WALLMESHVTX_Y; nCntZ++)
		{
			for (nCntX = 0; nCntX <= WALLMESHVTX_X; nCntX++)
			{
				//インデックスの設定
				pIdx[0] = (WALLMESHVTX_X + 1) * (nCntZ + 1) + nCntX;
				pIdx[1] = nCntX + (nCntZ * (WALLMESHVTX_X + 1));

				pIdx += 2;
			}
			if (nCntZ <= WALLMESHVTX_Y - 1)
			{
				pIdx[0] = (nCntX - 1) + (nCntZ * (WALLMESHVTX_X + 1));
				pIdx[1] = (nCntX - 1) + (nCntZ * (WALLMESHVTX_X + 1)) + (WALLMESHVTX_X + 2) * (nCntZ + 1);

				pIdx += 2;
			}
		}
		//インデックスバッファのアンロック
		g_pIdxBuffMeshWall->Unlock();
		////インデックスの設定(床)
		//pIdx[0] = 3;
		//pIdx[1] = 0;
		//
		//pIdx[2] = 4;
		//pIdx[3] = 1;
		//
		//pIdx[4] = 5;
		//pIdx[5] = 2;
		//
		//pIdx[6] = 2;
		//pIdx[7] = 6;
		//
		//pIdx[8] = 6;
		//pIdx[9] = 3;
		//
		//pIdx[10] = 7;
		//pIdx[11] = 4;
		//
		//pIdx[12] = 8;
		//pIdx[13] = 5;
		//
		////インデックスの設定(円柱)
		//pIdx[0] = 9;
		//pIdx[1] = 0;
		//
		//pIdx[2] = 10;
		//pIdx[3] = 1;
		//
		//pIdx[4] = 11;
		//pIdx[5] = 2;
		//
		//pIdx[6] = 12;
		//pIdx[7] = 3;
		//
		//pIdx[8] = 13;
		//pIdx[9] = 4;
		//
		//pIdx[10] = 14;
		//pIdx[11] = 5;
		//
		//pIdx[12] = 15;
		//pIdx[13] = 6;
		//
		//pIdx[14] = 16;
		//pIdx[15] = 7;
		//
		//pIdx[16] = 9;
		//pIdx[17] = 0;
	}
}
//===============================
// メッシュ壁の終了処理
//===============================
void UninitMeshWall(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}
//===============================
// メッシュ壁の更新処理
//===============================
void UpdateMeshWall(void)
{

}
//===============================
// メッシュ壁の描画処理
//===============================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshWall[nCnt].mtxWorld);

			//向きを反転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshWall[nCnt].rot.y, g_MeshWall[nCnt].rot.x, g_MeshWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_MeshWall[nCnt].pos.x, g_MeshWall[nCnt].pos.y, g_MeshWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);

			//メッシュ壁を描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, WALLMAX_VTX, 0, WALLPOLYGON_NO);
		}
	}
}
//===================
// メッシュ壁の設定
//===================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == false)
		{
			g_MeshWall[nCnt].pos = pos;
			g_MeshWall[nCnt].rot = rot;
			g_MeshWall[nCnt].bUse = true;
			break;
		}
	}
}