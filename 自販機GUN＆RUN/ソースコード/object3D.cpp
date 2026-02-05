//=================================
//
// ポリゴン表示処理[object3D.cpp]
// Author : Kaiti Aoto
//
//=================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
//================
// コンストラクタ
//================
CObject3D::CObject3D(int nPriority) :CObject(nPriority)
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_nIdxTex = 0;
}
//================
// デストラクタ
//================
CObject3D::~CObject3D()
{
}
//===========
// 生成処理
//===========
CObject3D* CObject3D::Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type)
{
	CObject3D* pObject3D;
	pObject3D = new CObject3D;
	//初期化
		//初期化
	if (FAILED(pObject3D->Init(pTexName, pos, rot, fWidth, fHeight, type)))
	{
		delete pObject3D;
		return nullptr;
	}
	
	return pObject3D;
}
//==================
// 初期化処理
//==================
HRESULT CObject3D::Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, TYPE type)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;	
	m_rot = rot;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_3D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	if (type == TYPE_FIELD || type == TYPE_SHADOW)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(- fWidth, 0.0f, + fHeight);
		pVtx[1].pos = D3DXVECTOR3(+ fWidth, 0.0f, + fHeight);
		pVtx[2].pos = D3DXVECTOR3(- fWidth, 0.0f, - fHeight);
		pVtx[3].pos = D3DXVECTOR3(+ fWidth, 0.0f, - fHeight);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	else if (type == TYPE_WALL)
	{
		m_pos.y += fWidth;
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fWidth, +fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+fWidth, +fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+fWidth, -fHeight, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	//pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y, m_pos.z + fHeight);
	//pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y+50, m_pos.z + fHeight);
	//pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y+50, m_pos.z - fHeight);
	//pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y, m_pos.z - fHeight);


	//D3DXVECTOR3 nor,nor3;
	//D3DXVec3Cross(&nor, &pVtx[1].pos, &pVtx[2].pos);
	//D3DXVec3Cross(&nor3, &pVtx[1].pos, &pVtx[2].pos);
	//D3DXVec3Normalize(&nor, &nor);
	//D3DXVec3Normalize(&nor3, &nor3);
	//pVtx[0].nor = nor;
	//pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//pVtx[3].nor = nor3;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[1].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[2].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
	pVtx[3].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);

	float texScaleX = fWidth / 100;
	float texScaleY = fHeight / 100;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(texScaleX, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, texScaleY);
	pVtx[3].tex = D3DXVECTOR2(texScaleX, texScaleY);

	if (type == TYPE_SHADOW)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(pTexName);

	return S_OK;
}
//==============
// 終了処理
//==============
void CObject3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CObject::Release();
}
//=============
// 更新処理
//=============
void CObject3D::Update(void)
{

}
//============
// 描画処理
//============
void CObject3D::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//テクスチャ取得
	CTexture* pTex = CManager::GetTex();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex));

	//ポリゴンを描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

float CObject3D::GetHeight(D3DXVECTOR3 pos)
{
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pos.x > pVtx[0].pos.x || pos.x < pVtx[1].pos.x||
		pos.z > pVtx[0].pos.z || pos.z < pVtx[2].pos.z)
	{
		D3DXVECTOR3 vec1, vec2, nor;
		float fHeight = 0.0f;

		vec1 = pVtx[1].pos - pVtx[0].pos;
		vec2 = pVtx[2].pos - pVtx[0].pos;

		D3DXVec3Cross(&nor, &vec1, &vec2);
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{
			//(pos.x - pVtx[0].pos.x)* nor.x + (fHeight - pVtx[0].pos.y) * nor.y + (pos.z - pVtx[0].pos.z) * nor.z = 0.0f;
			fHeight = -((pos.x - pVtx[0].pos.x) * nor.x + (pos.z - pVtx[0].pos.z) * nor.z) + pVtx[0].pos.y * nor.y;
			fHeight /= nor.y;
		}
		return fHeight;
	}

	return 0;
}

void CObject3D::SetColor(D3DXCOLOR col)
{
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
