//===============================================
//
// オブジェクト2D[object2D.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CObject2D::CObject2D(int nPriority):CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_fLength = 0.0f;
	m_fAngle = 0.0f;
	m_nIdxTex = 0;
	m_bUse = true;
}
//===============================================
// デストラクタ
//===============================================
CObject2D::~CObject2D()
{
}
//===============================================
// 生成処理
//===============================================
CObject2D* CObject2D::Create(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, int nPriority)
{
	CObject2D* pObject2D = new CObject2D(nPriority);
	// 初期化
	pObject2D->Init(pTexName, pos, rot, size);
	return pObject2D;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CObject2D::Init(const char* pTexName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 値を代入
	m_pos = pos;
	m_rot = rot;
	m_size = size;
	m_bUse = true;
	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) / 2.0f;
	m_fAngle = atan2f(m_size.x, m_size.y);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	// バッファ設定
	SetBuff(m_pVtxBuff);
	// 頂点座標設定
	Set(m_pos, m_rot);
	// カラー設定
	SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	// テクスチャ座標設定
	SetTex(0.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();
	m_nIdxTex = pTex->Register(pTexName);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CObject2D::Uninit(void)
{
	// 頂点バッファの破棄
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
void CObject2D::Update(void)
{
}
//===============================================
// 描画処理
//===============================================
void CObject2D::Draw(void)
{
	// デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	CTexture* pTex = CManager::GetTex();
	if (m_bUse == true)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		LPDIRECT3DTEXTURE9 tex = pTex->GetAddress(m_nIdxTex);
		if (tex != nullptr)
		{
			pDevice->SetTexture(0, tex);
		}
		else
		{
			pDevice->SetTexture(0, nullptr);
		}
		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//===============================================
// テクスチャ座標設定
//===============================================
void CObject2D::SetTex(float U1, float U2, float V1, float V2)
{
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(U1, V1);
	pVtx[1].tex = D3DXVECTOR2(U2, V1);
	pVtx[2].tex = D3DXVECTOR2(U1, V2);
	pVtx[3].tex = D3DXVECTOR2(U2, V2);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================
// カラー設定
//===============================================
void CObject2D::SetColor(D3DCOLOR fColor)
{
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].col = fColor;
	pVtx[1].col = fColor;
	pVtx[2].col = fColor;
	pVtx[3].col = fColor;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===============================================
// サイズ設定
//===============================================
void CObject2D::SetSize(D3DXVECTOR2 size)
{
	m_size = size;

	m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) / 2.0f;
	m_fAngle = atan2f(m_size.x, m_size.y);

	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;
	
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}
//===============================================
// 設定処理
//===============================================
void CObject2D::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	VERTEX_2D* pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_pos = pos;
	m_rot = rot;

	// 頂点座標
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (-D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (-D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * m_fLength;
	pVtx[3].pos.z = 0.0f;
	
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}