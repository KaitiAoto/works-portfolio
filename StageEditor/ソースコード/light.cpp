//==================================
//
//　ウインドウ表示処理[light.cpp]
//　Author:kaiti
//
//==================================
#include "light.h"
#include "renderer.h"
#include "manager.h"
//================
// コンストラクタ
//================
CLight::CLight(void)
{
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		m_light[nCnt] = {}; //ライト情報
	}
}
//================
// デストラクタ
//================
CLight::~CLight()
{
}
//==================
// ライトの初期化
//==================
HRESULT CLight::Init(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir[MAX_LIGHT];

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトをクリアする
		ZeroMemory(&m_light, sizeof(m_light));

		//ライトの種類を設定
		m_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;//平行光源

		//ライトの拡散光を設定
		m_light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//ライトの色

		//ライトの方向を設定
 		vecDir[0] = D3DXVECTOR3(-0.52f, -0.59f, 0.62f);
		vecDir[1] = D3DXVECTOR3(0.37f, -0.63f, -0.68f);
		vecDir[2] = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

		//vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		//vecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, 0.4f);
		//vecDir[2] = D3DXVECTOR3(0.0f, -0.8f, 0.0f);

		//正規化する（大きさ１のベクトルにする）
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_light[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_light[nCnt]);

		//ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}	

	return S_OK;
}
//====================
// ライトの終了処理
//====================
void CLight::Uninit(void)
{

}
//====================
// ライトの更新処理
//====================
void CLight::Update(void)
{

}