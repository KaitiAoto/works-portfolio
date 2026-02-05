//===============================================
//
// モデル表示処理[objectX.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_size = VECTOR3_NULL;
	m_realSize = VECTOR3_NULL;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_mtxWorld = {};

	m_pMesh = nullptr;		// メッシュ(頂点情報)へのポインタ
	m_pBuffMat = nullptr;	// マテリアルへのポインタ
	m_dwNumMat = 0;		// マテリアルの数
	m_nIdxTex = 0;
	m_nShakeTime = 0;
	m_MaxShake = VECTOR3_NULL;
	m_forcedColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDraw = true;
	m_bColorChange = false;
	m_bEmissive = false;
	m_bShake = false;
	D3DXQuaternionIdentity(&m_quat);
	m_bQuat = false;
}
//===============================================
// デストラクタ
//===============================================
CObjectX::~CObjectX()
{
}
//===============================================
// 生成処理
//===============================================
CObjectX* CObjectX::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CObjectX* pObjectX;
	pObjectX = new CObjectX(nPriority);
	//初期化
	pObjectX->Init(pFilename, pos, rot);
	return pObjectX;
}
//===============================================
//モデルの初期化
//===============================================
HRESULT CObjectX::Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CTexture* pTex = CManager::GetTex();

	m_pos = pos;
	m_OffPos = pos;
	m_rot = rot;
	m_bDraw = true;

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

	if (m_dwNumMat > 0)
	{
		//必要数のテクスチャへのIndexのポインタを動的確保
		m_nIdxTex = new int[m_dwNumMat];
	}
	else
	{
		m_nIdxTex = nullptr;
	}

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{
			int nIdx = pTex->Register(pMat[nCntMat].pTextureFilename);
			m_nIdxTex[nCntMat] = nIdx;  // インデックスを保存
		}
		else
		{
			m_nIdxTex[nCntMat] = -1;
		}

		// アルファを強制 1.0f にしておく
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
	}

	//-----------------------------------------------
	// モデルのサイズ設定
	//-----------------------------------------------
	int nNumVtx;   //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

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
	m_pMesh->UnlockVertexBuffer();

	m_size = D3DXVECTOR3(vtxMax.x - vtxMin.x, vtxMax.y - vtxMin.y, vtxMax.z - vtxMin.z);
	m_realSize = m_size;

	return S_OK;
}
//===============================================
//モデルの終了処理
//===============================================
void CObjectX::Uninit(void)
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
	if (m_nIdxTex != nullptr)
	{
		delete[] m_nIdxTex;
		m_nIdxTex = nullptr;
	}

	CObject::Release();
}
//===============================================
//モデルの更新処理
//===============================================
void CObjectX::Update(void)
{
	// 振動するなら
	if (m_bShake == true)
	{
		// 振動
		Shake();
	}

	UpdateOBB();
}
//===============================================
//モデルの描画
//===============================================
void CObjectX::Draw(void)
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

	if (m_bDraw == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

		//向きを反転
		if (m_bQuat == false)
		{
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		}
		else if(m_bQuat == true)
		{
			// クォータニオンから回転マトリックスを生成
			D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);
		}
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
			LPDIRECT3DTEXTURE9 tex = nullptr;
			// テクスチャを貼るなら
			if (m_nIdxTex && m_nIdxTex[nCntMat] >= 0)
			{
				tex = pTex->GetAddress(m_nIdxTex[nCntMat]);	// アドレス取得
			}

			// 色を変える
			D3DMATERIAL9 forcedMat = pMat[nCntMat].MatD3D;  // 既存のマテリアルをコピー
			forcedMat.Diffuse = m_forcedColor;
			forcedMat.Ambient = forcedMat.Diffuse;

			// 発光するなら
			if (m_bEmissive == true)
			{
				forcedMat.Emissive = forcedMat.Diffuse;
			}

			// 色変えするなら
			if (m_bColorChange == true)
			{
				// マテリアルを強制的に設定
				pDevice->SetMaterial(&forcedMat);
			}
			// 色変えしないなら
			else if (m_bColorChange == false)
			{
				forcedMat = pMat[nCntMat].MatD3D;  // 既存のマテリアルをコピー

				// 発光するなら
				if (m_bEmissive == true)
				{
					forcedMat.Emissive = pMat[nCntMat].MatD3D.Diffuse;
				}
				//マテリアルの設定
				pDevice->SetMaterial(&forcedMat);
			}
			// テクスチャ設定
			pDevice->SetTexture(0, tex);

			//モデルの描画
			m_pMesh->DrawSubset(nCntMat);
		}	

		pDevice->SetMaterial(&matDef);
	}
}
//===============================================
// OBBの更新
//===============================================
void CObjectX::UpdateOBB(void)
{
	m_obb.centerPos = m_pos;

	// モデルの回転を行列に
	D3DXMATRIX mRot;
	if (m_bQuat == false)
	{
		D3DXMatrixRotationYawPitchRoll(&mRot, m_rot.y, m_rot.x, m_rot.z);
	}
	else if (m_bQuat == true)
	{
		// クォータニオンから回転マトリックスを生成
		D3DXMatrixRotationQuaternion(&mRot, &m_quat);
	}


	// 各軸ベクトル
	m_obb.axis[0] = D3DXVECTOR3(mRot._11, mRot._12, mRot._13); // X軸
	m_obb.axis[1] = D3DXVECTOR3(mRot._21, mRot._22, mRot._23); // Y軸
	m_obb.axis[2] = D3DXVECTOR3(mRot._31, mRot._32, mRot._33); // Z軸

	// 半サイズ設定
	//m_obb.size = m_size * 0.5f;
	m_obb.size = m_realSize * 0.5f;
}
//===============================================
// モデル変更用処理
//===============================================
HRESULT CObjectX::ChangeModel(const char* pNewModel)
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
	if (m_nIdxTex != nullptr)
	{
		delete[] m_nIdxTex;
		m_nIdxTex = 0;
	}

	m_dwNumMat = 0;

	// 新しいモデルとして再読み込み
	return Init(pNewModel, m_pos, m_rot);
}
//===============================================
// 位置＆角度設定
//===============================================
void CObjectX::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
}
//===============================================
// 振動処理
//===============================================
void CObjectX::Shake(void)
{
	// カウンターを加算
	m_nShakeTime--;

	// カウンターが０じゃないなら
	if (m_nShakeTime > 0)
	{
		D3DXVECTOR3 move = VECTOR3_NULL;	// 移動量
		// 各軸ごとに振動
		move.x = ((rand() % 200) - 100) / 100.0f * m_MaxShake.x;
		move.y = ((rand() % 200) - 100) / 100.0f * m_MaxShake.y;
		move.z = ((rand() % 200) - 100) / 100.0f * m_MaxShake.z;

		// 値を加算
		m_pos += move;
	}
	// カウンターが０以下なら
	else if (m_nShakeTime <= 0)
	{
		m_nShakeTime = 0;
		m_bShake = false;
		m_pos = m_OffPos;
	}
}
//===============================================
// 実際にしたいサイズにする
//===============================================
void CObjectX::SetRealSize(D3DXVECTOR3 size)
{
	// 0が指定された場合は元のサイズにする
	m_realSize.x = (size.x == 0.0f) ? m_size.x : size.x;
	m_realSize.y = (size.y == 0.0f) ? m_size.y : size.y;
	m_realSize.z = (size.z == 0.0f) ? m_size.z : size.z;

	// スケールを算出
	m_scale.x = (m_size.x != 0.0f) ? m_realSize.x / m_size.x : 1.0f;
	m_scale.y = (m_size.y != 0.0f) ? m_realSize.y / m_size.y : 1.0f;
	m_scale.z = (m_size.z != 0.0f) ? m_realSize.z / m_size.z : 1.0f;
}
//===============================================
// クォータニオン設定
//===============================================
void CObjectX::SetQuat(bool bQuat, const D3DXQUATERNION& quat)
{
	m_bQuat = bQuat;

	// 差分回転を積算
	m_quat = quat * m_quat;

	// 正規化
	D3DXQuaternionNormalize(&m_quat, &m_quat);
}
//===============================================
// 回転を戻す
//===============================================
void CObjectX::ReturnRotation(void)
{
	m_bQuat = false;

	// 目標：回転なし
	D3DXQUATERNION target(0, 0, 0, 1);

	// 1フレームあたりの戻り率
	float t = 0.25f;
	if (t > 1.0f) t = 1.0f;

	D3DXQuaternionSlerp(&m_quat, &m_quat, &target, t);
	D3DXQuaternionNormalize(&m_quat, &m_quat);
}
