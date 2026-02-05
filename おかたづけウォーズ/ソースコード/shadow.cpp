//===============================================
//
// 影[shadow.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "shadow.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CShadow::CShadow(int nPriority):CObject3D(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;

	m_fLength = 0;
	m_fRadius = 0;

	m_bUse = true;
	m_nIdxTex = 0;
	m_type = TYPE_SQUARE;
}
//===============================================
// デストラクタ
//===============================================
CShadow::~CShadow()
{
}
//===============================================
// 生成処理
//===============================================
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fX, float fZ, TYPE type)
{
	CShadow* pEffect;
	//
	pEffect = new CShadow;

	pEffect->m_type = type;
	//初期化
	if (FAILED(pEffect->Init(pos, rot, fX, fZ)))
	{
		delete pEffect;
		return nullptr;
	}
	return pEffect;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fX, float fZ)
{
	m_pos = pos;
	m_rot = rot;
	m_bUse = true;
	if (m_type == TYPE_SQUARE)
	{
		CObject3D::Init("data\\TEXTURE\\shadow001.jpg", m_pos, rot, fX, fZ, CObject3D::TYPE_SHADOW);
	}
	else
	{
		CObject3D::Init("data\\TEXTURE\\shadow000.jpg", m_pos, rot, fX, fZ, CObject3D::TYPE_SHADOW);
	}
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	SetObjType(CObject::TYPE_STAGE);
	
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CShadow::Uninit(void)
{
	CObject3D::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CShadow::Update(void)
{
	if (m_bUse == true)
	{
		SetPos(m_pos);
	}
	else if (m_bUse == false)
	{//使っていないなら
		Uninit();
	}
}
//===============================================
// 描画処理
//===============================================
void CShadow::Draw(void)
{
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_bUse == true)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		CObject3D::Draw();

		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//設定を元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}