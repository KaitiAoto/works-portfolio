//===============================================
//
// レンダリング[renderer.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"

//===============================================
// コンストラクタ
//===============================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;

	m_pTexMT = nullptr;
	m_pRendMT = nullptr;
	m_pZBuffMT = nullptr;
	m_viewportMT = {};

}
//===============================================
// デストラクタ
//===============================================
CRenderer::~CRenderer()
{

}
//===============================================
// 初期化処理
//===============================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm{};
	D3DPRESENT_PARAMETERS d3dpp{};

	//Direct3Dオブジェ生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	CDebugProc* pDegub = CManager::GetDebug();

	pDegub->Init();


	//{
	//	LPDIRECT3DSURFACE9 pRendDef, pZBuffDef;
	//	//
	//	m_pD3DDevice->CreateTexture(SCREEN_WIDTH,
	//		SCREEN_HEIGHT,
	//		1,
	//		D3DUSAGE_RENDERTARGET,
	//		D3DFMT_A8R8G8B8,
	//		D3DPOOL_DEFAULT,
	//		&m_pTexMT,
	//		NULL);
	//	//
	//	m_pTexMT->GetSurfaceLevel(0, &m_pRendMT);
	//	//
	//	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH,
	//		SCREEN_HEIGHT,
	//		D3DFMT_D16,
	//		D3DMULTISAMPLE_NONE,
	//		0,
	//		TRUE,
	//		&m_pZBuffMT,
	//		NULL);
	//
	//	m_pD3DDevice->GetRenderTarget(0, &pRendDef);
	//	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);
	//	m_pD3DDevice->SetRenderTarget(0, m_pRendMT);
	//	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);
	//	//m_pD3DDevice->Clear();
	//	m_pD3DDevice->SetRenderTarget(0, pRendDef);
	//	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	//
	//
	//	//
	//	m_viewportMT.X = 0;
	//	m_viewportMT.Y = 0;
	//	m_viewportMT.Width = SCREEN_WIDTH;
	//	m_viewportMT.Height = SCREEN_HEIGHT;
	//	m_viewportMT.MinZ = 0.0f;
	//	m_viewportMT.MaxZ = 1.0f;
	//
	//}

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CRenderer::Uninit(void)
{
	CDebugProc* pDegub = CManager::GetDebug();

	pDegub->Uninit();

	//D3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
	//D3Dオブジェの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}
//===============================================
// 更新処理
//===============================================
void CRenderer::Update(void)
{
	CObject::UpdateAll();
}
//===============================================
// 描画処理
//===============================================
void CRenderer::Draw(void)
{
	CDebugProc* pDegub = CManager::GetDebug();

	//画面クリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CObject::DrawAll();
		
		if (CManager::GetPause() == true)
		{
			CPause* pPause = CGame::GetPause();
			if (pPause != nullptr)
			{
				pPause->Draw();
			}
		}


		pDegub->Draw();

		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			pFade->Draw();
		}


		//描画終了
		m_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//===============================================
//
//===============================================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 VecU)
{
	D3DXMATRIX mtxview, mtxProjection;
	float fAspect;

	//
	m_pD3DDevice->SetRenderTarget(0, m_pRendMT);
	//
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);
	//
	m_pD3DDevice->SetViewport(&m_viewportMT);
	//ビューマトリックス初期化

	//ビューマトリックス作成
	D3DXMatrixLookAtLH(&mtxview, &posV, &posR, &VecU);
	//ビューマトリックス設定

	//プロジェクションマトリックス初期化
	
	//プロジェクションマトリックス作成
	fAspect = (float)m_viewportMT.Width / (float)m_viewportMT.Height;
	D3DXMatrixLookAtLH(&mtxProjection, &posV, &posR, &VecU);
	//プロジェクションマトリックス設定

}
