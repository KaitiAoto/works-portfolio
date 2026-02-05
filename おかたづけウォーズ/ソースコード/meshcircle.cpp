//===============================================
//
// メッシュ円表示処理[MeshCircle.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "meshcircle.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CMeshCircle::CMeshCircle(int nPriority):CObject(nPriority)
{
	// 値をクリア
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_nIdxTex = 0;
	m_type = TYPE_IN;
	m_bUse = true;
}
//===============================================
// デストラクタ
//===============================================
CMeshCircle::~CMeshCircle()
{
}
//===============================================
// 生成処理
//===============================================
CMeshCircle* CMeshCircle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int vtx_x, int vtx_z)
{
	// インスタンス生成
	CMeshCircle* pMesh = new CMeshCircle;

	//初期化
	if (FAILED(pMesh->Init(pos, rot, fRadius, col, type, vtx_x, vtx_z)))
	{
		delete pMesh;
		return nullptr;
	}

	return pMesh;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CMeshCircle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, D3DXCOLOR col, TYPE type, int vtx_x, int vtx_z)
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

	// 値を代入
	m_pos = pos;
	m_rot = rot;
	m_fRadius = fRadius;
	m_type = type;
	m_bUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VTX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);

	int stripCount = (VTX_X + 1) * 2 + 2;			// 1ストリップのインデックス数
	int stripTotal = VTX_X;							// ストリップ数（Z方向）
	int nIndexCount = stripCount * stripTotal;		// インデックス数カウンター

	// 両面表示なら
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

	int index = 0;									// インデックス番号
	float fBaseAngle = (D3DX_PI * 2.0f) / VTX_X;	// 基準角度

	for (int nCntV = 0; nCntV < VTX_Z + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < VTX_X + 1; nCntH++)
		{
			// 角度
			float fAngle = fBaseAngle * nCntH;

			// 頂点座標の設定
			pVtx[index].pos = D3DXVECTOR3(cosf(fAngle) * m_fRadius, 0.0f, sinf(fAngle) * m_fRadius);
			
			D3DXVECTOR3 nor;	// 位置
			nor = pVtx[index].pos - m_pos;

			// 法線ベクトル
			D3DXVec3Normalize(&pVtx[index].nor,&nor);

			// 頂点カラーの設定
			pVtx[index].col = col;

			// テクスチャ座標の設定
			pVtx[index].tex = D3DXVECTOR2((float)nCntH, (float)nCntV / 2);

			index++;
		}
	}

	// 頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	// インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	
	int nCntX;
	int nIdx = 0;	// インデックス番号

	for (int nCntZ = 0; nCntZ < VTX_Z; nCntZ++)
	{
		for (nCntX = 0; nCntX <= VTX_X; nCntX++)
		{
			// 現在のZ列と次のZ列の頂点インデックス
			int i0 = nCntX + (nCntZ * (VTX_X + 1));
			int i1 = nCntX + ((nCntZ + 1) * (VTX_X + 1));

			// 内側
			if (type == TYPE_IN)
			{
				// 裏向き
				pIdx[nIdx++] = (WORD)i1;
				pIdx[nIdx++] = (WORD)i0;
			}
			// 外側or両面
			else if (type == TYPE_OUT || type == TYPE_BOTHSIDES)
			{
				// 表向き
				pIdx[nIdx++] = (WORD)i0;
				pIdx[nIdx++] = (WORD)i1;
			}
		}

		// ディグナルトライアングルでストリップを区切る
		if (nCntZ < VTX_Z - 1)
		{
			// 現在ストリップの最後の頂点
			int i = (nCntX - 1) + (nCntZ * (VTX_X + 1));
			
			// デジェネレート用インデックス
			pIdx[nIdx++] = (WORD)i;
			pIdx[nIdx++] = (WORD)i;
		}
	}

	// 両面描画の内側分を追加（順番逆でインデックス再構築）
	if (type == TYPE_BOTHSIDES)
	{
		for (int nCntZ = 0; nCntZ < VTX_Z; nCntZ++)
		{
			for (nCntX = 0; nCntX <= VTX_X; nCntX++)
			{
				// 現在のZ列と次のZ列の頂点インデックス
				int i0 = nCntX + (nCntZ * (VTX_X + 1));
				int i1 = nCntX + ((nCntZ + 1) * (VTX_X + 1));

				// 裏向き
				pIdx[nIdx++] = (WORD)i1;
				pIdx[nIdx++] = (WORD)i0;
			}

			// 次のストリップへ
			if (nCntZ < VTX_Z - 1)
			{
				int i = (nCntX - 1) + (nCntZ * (VTX_X + 1));
				pIdx[nIdx++] = (WORD)i;
				pIdx[nIdx++] = (WORD)i;
			}
		}
	}

	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}
//===============================================
// メッシュ円柱の終了処理
//===============================================
void CMeshCircle::Uninit(void)
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
// メッシュ円柱の更新処理
//===============================================
void CMeshCircle::Update(void)
{
}
//===============================================
// メッシュ円柱の描画処理
//===============================================
void CMeshCircle::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	if (m_bUse == true)
	{
		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		// ALPHAテストの設定
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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
		pDevice->SetTexture(0, nullptr);

		DWORD oldCullMode;
		// 現在のカリング設定を保存
		pDevice->GetRenderState(D3DRS_CULLMODE, &oldCullMode);

		// タイプに応じてカリング設定を切り替え
		switch (m_type)
		{
			// 外側
		case TYPE_OUT:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			break;

			// 内側
		case TYPE_IN:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			break;

			// 両面
		case TYPE_BOTHSIDES:
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			break;
		}
		// メッシュ円柱を描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, MAX_VTX, 0, POLYGON_NO);

		// 元のカリング設定に戻す
		pDevice->SetRenderState(D3DRS_CULLMODE, oldCullMode);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}
//===============================================
// サイズ変更
//===============================================
void CMeshCircle::SetSize(float fRadius)
{
	// 半径を代入
	m_fRadius = fRadius;

	VERTEX_3D* pVtx = NULL;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int index = 0;									// インデックス番号
	float fBaseAngle = (D3DX_PI * 2.0f) / VTX_X;	// 基準角度

	for (int nCntV = 0; nCntV < VTX_Z + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < VTX_X + 1; nCntH++)
		{
			// 角度
			float fAngle = fBaseAngle * nCntH;

			// 頂点座標の設定
			pVtx[index].pos = D3DXVECTOR3(cosf(fAngle) * m_fRadius, 0.0f, sinf(fAngle) * m_fRadius);

			D3DXVECTOR3 nor;	// 位置
			nor = pVtx[index].pos - m_pos;

			// 法線ベクトル
			D3DXVec3Normalize(&pVtx[index].nor, &nor);

			index++;
		}
	}

	// 頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

}
//===============================================
// 色変え
//===============================================
void CMeshCircle::SetColor(D3DXCOLOR col)
{
	VERTEX_3D* pVtx = NULL;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int index = 0;	// インデックス番号

	for (int nCntV = 0; nCntV < VTX_Z + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < VTX_X + 1; nCntH++)
		{
			// 頂点カラーの設定
			pVtx[index].col = col;

			index++;
		}
	}

	// 頂点バッファをアンロック　
	m_pVtxBuff->Unlock();
}