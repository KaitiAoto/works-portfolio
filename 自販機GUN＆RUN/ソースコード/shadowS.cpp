//==============================
//
// 影[shadow.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "shadowS.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CShadowS::CShadowS(int nPriority) :CObjectX(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fLength = 0;
	m_fRadius = 0;

	m_bUse = true;
	m_nIdxTex = 0;
	m_pObj2D = nullptr;
}
//================
// デストラクタ
//================
CShadowS::~CShadowS()
{
}
//===========
// 生成処理
//===========
CShadowS* CShadowS::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	CShadowS* pEffect;
	//
	pEffect = new CShadowS;
	//初期化
	if (FAILED(pEffect->Init(pos, rot, fRadius)))
	{
		delete pEffect;
		return nullptr;
	}

	return pEffect;
}
//===============
// 初期化処理
//===============
HRESULT CShadowS::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	m_pos = pos;
	m_rot = rot;
	m_fRadius = fRadius;
	m_fLength=(sqrtf(fRadius * fRadius + fRadius * fRadius) / 2.0f);

	m_pObj2D = CObject2D::Create("data\\TEXTURE\\shadow000.jpg", D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), m_rot, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (m_pObj2D)
	{
		m_pObj2D->SetColor(D3DCOLOR_RGBA(1, 1, 1, 255)); // 半透明
	}

	CObjectX::Init(m_pos, m_rot);

	SetObjType(CObject::TYPE_SHADOW);
	
	return S_OK;
}
//============
// 終了処理
//============
void CShadowS::Uninit(void)
{
	CObjectX::Uninit();
}
//============
// 更新処理
//============
void CShadowS::Update(void)
{
	CObjectX::Set(m_pos, m_rot);
}
//============
// 描画処理
//============
void CShadowS::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();


	//ステンシルテストを有効に
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	//Zバッファへの書き込み無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	//カラーバッファへの書き込み無効
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);
	//ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//表面カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//影モデル描画
	CObjectX::Draw();

	//ステンシルバッファの参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	//ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//裏面カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//影モデル描画
	CObjectX::Draw();

	//カラーバッファへの書き込み有効
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F); // カラー描画ON
	//ステンシルバッファの参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);
	//ステンシルバッファの比較パラメータ設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	//黒ポリゴン描画
	m_pObj2D->Draw();

	//全て元に戻す

	//Zバッファへの書き込み有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//ステンシルテストを無効に
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}