//==============================
//
// 自販機[vender.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "stage.h"
#include "renderer.h"
#include "manager.h"
//==================
// コンストラクタ
//==================
CStage::CStage(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_mtxWorld = {};

	m_pModel = NULL;
}
//================
// デストラクタ
//================
CStage::~CStage()
{

}
//===========
// 生成処理
//===========
CStage* CStage::Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CStage* pBullet = new CStage;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pFilename, pos, rot)))
	{
		delete pBullet;
		return nullptr;
	}

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CStage::Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;

	//モデル生成
	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	m_size = m_pModel->SetSize();

	CShadow::Create(m_pos, m_rot, m_size.x, m_size.z);

	//オブジェクトの種類設定
	SetObjType(TYPE_STAGE);

	return S_OK;
}
//============
// 終了処理
//============
void CStage::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}
	CObject::Release();
}
//============
// 更新処理
//============
void CStage::Update(void)
{
	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{
		bool bColl = false;

		bColl = CGame::GetColl()->CameraToStage(this);
		if (bColl == true)
		{
			m_pModel->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f));
		}
		m_pModel->SetColorChange(bColl);
	}
}
//============
// 描画処理
//============
void CStage::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル描画
	m_pModel->Draw();
}