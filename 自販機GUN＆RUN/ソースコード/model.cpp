//================================
//
// モデル表示処理[objectX.cpp]
// Author : Kaiti Aoto
//
//================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "motion.h"
#include "debugproc.h"
//================
// コンストラクタ
//================
CModel::CModel(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Offsetpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Offsetrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_mtxWorld = {};

	m_pMesh = NULL;		//メッシュ(頂点情報)へのポインタ
	m_pBuffMat = NULL;	//マテリアルへのポインタ
	m_dwNumMat = 0;		//マテリアルの数
	m_nIdxTex = 0;
	m_pParent = NULL;

	m_forcedColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bColorChange = false;
}
//================
// デストラクタ
//================
CModel::~CModel()
{
}
//===========
// 生成処理
//===========
CModel* CModel::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel* pObjectX;
	pObjectX = new CModel;
	//初期化
	pObjectX->Init(pFilename, pos, rot);
	return pObjectX;
}
//================
//モデルの初期化
//================
HRESULT CModel::Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CTexture* pTex = CManager::GetTex();

	m_pos = pos;
	m_rot = rot;

	//Xファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(pFilename,
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
	for (DWORD i = 0; i < m_dwNumMat; i++) 
	{
		m_nIdxTex[i] = -1;  // 初期化
	}

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			int nIdx = pTex->Register(pMat[nCntMat].pTextureFilename);

			if (nIdx < 0)
			{//エラーチェック
				char msg[256];
				sprintf(msg, "テクスチャの登録に失敗しました: %s\n", pMat[nCntMat].pTextureFilename);
				OutputDebugStringA(msg);
				m_nIdxTex[nCntMat] = -1;
			}
			else
			{
				m_nIdxTex[nCntMat] = nIdx;  // インデックスを保存
			}
		}
	}

	return S_OK;
}
//==================
//モデルの終了処理
//==================
void CModel::Uninit(void)
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
		m_nIdxTex = NULL;
	}
}
//=================
//モデルの更新処理
//=================
void CModel::Update(void)
{

}
//==============
//モデルの描画
//===============
void CModel::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//テクスチャ取得
	CTexture* pTex = CManager::GetTex();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	//マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	//向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_Offsetrot.y, m_rot.x + m_Offsetrot.x, m_rot.z + m_Offsetrot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_Offsetpos.x, m_pos.y + m_Offsetpos.y, m_pos.z + m_Offsetpos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//親のマトリックスと掛け合わせる
	D3DXMATRIX mtxParent;
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を変える
	D3DMATERIAL9 forcedMat = pMat[0].MatD3D;  // 既存のマテリアルをコピー
	forcedMat.Diffuse = m_forcedColor;
	forcedMat.Ambient = forcedMat.Diffuse;

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		LPDIRECT3DTEXTURE9 tex = nullptr;
		if (m_nIdxTex && m_nIdxTex[nCntMat] >= 0)
		{
			tex = pTex->GetAddress(m_nIdxTex[nCntMat]);
		}

		if (m_bColorChange == true)
		{
			// マテリアルを強制的に設定
			pDevice->SetMaterial(&forcedMat);
		}
		else if(m_bColorChange == false)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		pDevice->SetTexture(0, tex);

		//モデルの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	pDevice->SetMaterial(&matDef);
}
void CModel::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
}
D3DXVECTOR3 CModel::SetSize(void)
{
	int nNumVtx;   //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの最小値
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの最大値

	//頂点数取得
	nNumVtx = m_pMesh->GetNumVertices();
	//頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//頂点座標を比較してプレイヤーの最小値、最大値を取得
		//最小値
		if (vtx.x < vtxMin.x)
		{
			vtxMin.x = vtx.x;
		}
		if (vtx.y < vtxMin.y)
		{
			vtxMin.y = vtx.y;
		}
		if (vtx.z < vtxMin.z)
		{
			vtxMin.z = vtx.z;
		}
		//最大値
		if (vtx.x > vtxMax.x)
		{
			vtxMax.x = vtx.x;
		}
		if (vtx.y > vtxMax.y)
		{
			vtxMax.y = vtx.y;
		}
		if (vtx.z > vtxMax.z)
		{
			vtxMax.z = vtx.z;
		}
		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	D3DXVECTOR3 size = D3DXVECTOR3(vtxMax.x - vtxMin.x, vtxMax.y - vtxMin.y, vtxMax.z - vtxMin.z);

	return size;
}
//=================
// モデル読み込み
//=================
void CModel::Load(const char* pFilename, CModel** model, int& outNumModel)
{
	D3DXVECTOR3 pos[NUM_MODEL];
	D3DXVECTOR3 rot[NUM_MODEL];

	for (int n = 0; n < NUM_MODEL; n++) {
		pos[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	int nNumModel = 0;
	int Index = 0;
	int nNum_Parts = 0;
	int nCntModel = 0;
	int nParent = 0;

	FILE* pFile = fopen(pFilename, "r");
	if (!pFile) return;

	char aString[MAX_WORD];
	fscanf(pFile, "%s", &aString[0]);

	if (strcmp(aString, "SCRIPT") == 0)
	{
		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);
			if (aString[0] == '#') { fgets(aString, MAX_WORD, pFile); continue; }

			if (strcmp(aString, "NUM_MODEL") == 0)
			{
				fscanf(pFile, "%d", &nNumModel);
				outNumModel = nNumModel;
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &aString[0]);
				model[nCntModel] = CModel::Create(aString, pos[nCntModel], rot[nCntModel]);
				nCntModel++;
			}
			else if (strcmp(aString, "CHARACTERSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);
					if (aString[0] == '#') { fgets(aString, MAX_WORD, pFile); continue; }

					if (strcmp(aString, "NUM_PARTS") == 0)
					{
						fscanf(pFile, "%d", &nNum_Parts);
					}
					else if (strcmp(aString, "PARTSSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aString[0]);
							if (aString[0] == '#') { fgets(aString, MAX_WORD, pFile); continue; }

							if (strcmp(aString, "INDEX") == 0)
							{
								fscanf(pFile, "%d", &Index);
							}
							else if (strcmp(aString, "PARENT") == 0)
							{
								fscanf(pFile, "%d", &nParent);
								model[Index]->SetParent(model[nParent]);
							}
							else if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%f", &pos[Index].x);
								fscanf(pFile, "%f", &pos[Index].y);
								fscanf(pFile, "%f", &pos[Index].z);
							}
							else if (strcmp(aString, "ROT") == 0)
							{
								fscanf(pFile, "%f", &rot[Index].x);
								fscanf(pFile, "%f", &rot[Index].y);
								fscanf(pFile, "%f", &rot[Index].z);
							}
							else if (strcmp(aString, "END_PARTSSET") == 0)
							{
								model[Index]->SetOffSet(pos[Index], rot[Index]);
								break;
							}
						}
					}
					else if (strcmp(aString, "END_CHARACTERSET") == 0)
					{
						break;
					}
				}
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	fclose(pFile);
}