//==============================
//
//  ゲージ処理[Gauge.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
//==================
// コンストラクタ
//==================
CGauge::CGauge()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = NULL;
	m_nIdxTex = 0;
	m_Base = 0;
	m_fHeight = 0;
}
//================
// デストラクタ
//================
CGauge::~CGauge()
{
}
//===========
// 生成処理
//===========
CGauge* CGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CGauge* pGause;
	//
	pGause = new CGauge;
	//初期化
	if (FAILED(pGause->Init(pos, base, fHeight,col)))
	{
		delete pGause;
		return nullptr;
	}

	//テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	pGause->m_nIdxTex = pTex->Register("data\\TEXTURE\\gauge00.jpeg");

	return pGause;
}
//=============
// 初期化処理
//=============
HRESULT CGauge::Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_Base = base;
	m_fHeight = fHeight;
	m_col = col;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &m_pVtxBuff,
		                        NULL);
	Set();

	return S_OK;
}
//==========
// 終了処理
//==========
void CGauge::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//==========
// 更新処理
//==========
void CGauge::Update(void)
{

}
//===========
// 描画処理
//===========
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
//
//
//
void CGauge::Set()
{
	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (GAUGE_X * m_Base), m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (GAUGE_X * m_Base), m_pos.y + m_fHeight, 0.0f);
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