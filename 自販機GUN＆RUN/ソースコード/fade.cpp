//================================
//
// タイトル処理[title.cpp]
// Author : Kaiti Aoto
//
//================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"

//==================
// コンストラクタ
//==================
CFade::CFade()
{
	
}
//================
// デストラクタ
//================
CFade::~CFade()
{
}
//
//
//
CFade* CFade::Create(CScene::MODE modeNext)
{
	CFade* pFade = new CFade;

	if (pFade)
	{
		pFade->Init(modeNext);
	}

	return pFade;
}
void CFade::Set(CScene::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
}
//=======================
// 初期化
//=======================
void CFade::Init(CScene::MODE modeNext)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	m_modeNext = modeNext;
}
//=============
// 終了処理
//=============
void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=======================
// 更新
//=======================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//フェードイン
			m_col.a -= 0.03f;
			if (m_col.a <= 0.0f)
			{
				m_col.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{//フェードアウト
			m_col.a += 0.03f;
			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
				m_fade = FADE_IN;
				//モードの設定
				CManager::SetMode(m_modeNext);
				return;
			}
		}
		SetColor(m_col);
	}
}
//=======================
// 描画
//=======================
void CFade::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
void CFade::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}
