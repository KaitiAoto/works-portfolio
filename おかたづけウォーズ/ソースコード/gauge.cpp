//===============================================
//
//  ゲージ処理[Gauge.cpp]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "gauge.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CGauge::CGauge(int nPriority) : CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_pVtxBuff = nullptr;
	m_nIdxTex = 0;
	m_Base = 0;
}
//===============================================
// デストラクタ
//===============================================
CGauge::~CGauge()
{
}
//===============================================
// 生成処理
//===============================================
CGauge* CGauge::Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size)
{
	CGauge* pGause = nullptr;
	//
	pGause = new CGauge;
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
HRESULT CGauge::Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR2 size)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_Base = base;
	m_MaxBase = base;
	m_col = col;
	m_size = size;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);
	Set(m_Base, m_col);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CGauge::Uninit(void)
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
void CGauge::Update(void)
{

}
//===============================================
// 描画処理
//===============================================
void CGauge::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	//テクスチャ取得
	CTexture* pTex = CManager::GetTex();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	
	//テクスチャの設定
	pDevice->SetTexture(0, pTex->GetAddress(m_nIdxTex));
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//===============================================
// ゲージの枠を生成
//===============================================
void CGauge::CreateFrame(const char* pFileName)
{
	CObject2D::Create(pFileName, D3DXVECTOR3(m_pos.x + (m_size.x * m_MaxBase / 2), m_pos.y, m_pos.z), VECTOR3_NULL, D3DXVECTOR2(m_size.x * m_MaxBase, m_size.y * 3.0f), static_cast<int>(Priority::UI_Front));
}
//===============================================
// ゲージの枠を生成
//===============================================
void CGauge::CreateBack(const char* pFileName, D3DXCOLOR col)
{	 
	CObject2D* pObj2D = CObject2D::Create(pFileName, D3DXVECTOR3(m_pos.x + (m_size.x * m_MaxBase / 2), m_pos.y, m_pos.z), VECTOR3_NULL, D3DXVECTOR2(m_size.x * m_MaxBase, m_size.y * 3.0f), static_cast<int>(Priority::UI_Back));
	pObj2D->SetColor(col);
}
//===============================================
// 設定
//===============================================
void CGauge::Set(float fBase, D3DXCOLOR col)
{
	m_Base = fBase;
	m_col = col;

	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x * m_Base), m_pos.y - m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x * m_Base), m_pos.y + m_size.y, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
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