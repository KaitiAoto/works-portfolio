//===============================================
//
// ビルボード表示処理[billboard.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "objectBillboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CObjectBillboard::CObjectBillboard(int nPriority):CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxWorld = {};
	m_nIdxTex = 0;
	m_bDraw = true;
}
//===============================================
// デストラクタ
//===============================================
CObjectBillboard::~CObjectBillboard()
{
}
//===============================================
// 生成処理
//===============================================
CObjectBillboard* CObjectBillboard::Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nPriority)
{
	CObjectBillboard* pObject2D;
	pObject2D = new CObjectBillboard(nPriority);
	//初期化
		//初期化
	if (FAILED(pObject2D->Init(pTexName, pos, rot, fWidth, fHeight)))
	{
		delete pObject2D;
		return nullptr;
	}

	return pObject2D;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CObjectBillboard::Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_rot = rot;
	m_bDraw = true;


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

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(- fWidth, + fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+ fWidth, + fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(- fWidth, - fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+ fWidth, - fHeight, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(pTexName);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CObjectBillboard::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CObjectBillboard::Update(void)
{

}
//===============================================
// 描画処理
//===============================================
void CObjectBillboard::Draw(void)
{
	if (m_bDraw == true)
	{
		//デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		//テクスチャ取得
		CTexture* pTex = CManager::GetTex();
		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		//ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		D3DXMATRIX mtxView;
		//ビューマトリックス取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//カメラの逆行列を設定
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;

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

		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//ライトを有効に戻す
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}
//===============================================
// 設定処理
//===============================================
void CObjectBillboard::SetSize(float fRadius)
{
	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x-fRadius, m_pos.y+fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x+fRadius, m_pos.y+fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x -fRadius, m_pos.y-fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x+fRadius, m_pos.y-fRadius, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================
// 色設定
//===============================================
void CObjectBillboard::SetColor(D3DXCOLOR col)
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