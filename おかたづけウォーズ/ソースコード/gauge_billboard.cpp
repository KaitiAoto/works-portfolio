//===============================================
//
//  ビルボードゲージ処理[gauge_billboard.h]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "gauge_billboard.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "objectBillboard.h"

//===============================================
// コンストラクタ
//===============================================
CGaugeBillboard::CGaugeBillboard(int nPriority) : CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_pVtxBuff = nullptr;
	m_nIdxTex = 0;
	m_fBase = 0;
}
//===============================================
// デストラクタ
//===============================================
CGaugeBillboard::~CGaugeBillboard()
{
}
//===============================================
// 生成処理
//===============================================
CGaugeBillboard* CGaugeBillboard::Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size)
{
	CGaugeBillboard* pGause;
	//
	pGause = new CGaugeBillboard;
	//初期化
	if (FAILED(pGause->Init(pos, base, col, size)))
	{
		delete pGause;
		return nullptr;
	}

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	pGause->m_nIdxTex = pTex->Register("data\\TEXTURE\\gauge00.jpeg");

	return pGause;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CGaugeBillboard::Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_fBase = base;
	m_fMaxBase = base;
	m_col = col;
	m_size = size;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	Set(m_fBase, m_col);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CGaugeBillboard::Uninit(void)
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
void CGaugeBillboard::Update(void)
{

}
//===============================================
// 描画処理
//===============================================
void CGaugeBillboard::Draw(void)
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
//===============================================
// 設定
//===============================================
void CGaugeBillboard::Set(float fBase, D3DXCOLOR col)
{
	m_fBase = fBase;
	m_col = col;

	VERTEX_3D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_fRate = m_fBase / m_fMaxBase;
	m_fRate = max(0.0f, min(m_fRate, 1.0f));

	float fullHalfWidth = (m_size.x * m_fMaxBase) / 2.0f;
	float currentHalfWidth = fullHalfWidth * m_fRate;

	// 右端固定、左端を右側へ移動させて縮める
	pVtx[0].pos = D3DXVECTOR3(-fullHalfWidth, m_size.y, 0.0f);								// 左上
	pVtx[1].pos = D3DXVECTOR3(-fullHalfWidth + currentHalfWidth * 2.0f, m_size.y, 0.0f);	// 右上
	pVtx[2].pos = D3DXVECTOR3(-fullHalfWidth, -m_size.y, 0.0f);								// 左下
	pVtx[3].pos = D3DXVECTOR3(-fullHalfWidth + currentHalfWidth * 2.0f, -m_size.y, 0.0f);	// 右下

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================
// ゲージの枠を生成
//===============================================
void CGaugeBillboard::CreateFrame(const char* pFileName)
{
	CObjectBillboard::Create(pFileName, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), VECTOR3_NULL, m_size.x * m_fMaxBase / 1.9f, m_size.y * 1.5f, static_cast<int>(Priority::StageUI));
}
//===============================================
// ゲージの枠を生成
//===============================================
void CGaugeBillboard::CreateBack(const char* pFileName, D3DXCOLOR col)
{
	CObjectBillboard* pBack = CObjectBillboard::Create(pFileName, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), VECTOR3_NULL, m_size.x * m_fMaxBase / 2.0f, m_size.y, static_cast<int>(Priority::StageUI) - 1);
	pBack->SetColor(col);
}