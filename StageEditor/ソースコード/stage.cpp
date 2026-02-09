//==============================
//
// 自販機[vender.cpp]
// Author:kaiti
//
//==============================
#include "stage.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "savestage.h"
//
int CStage::m_nCntModel = 0;
int CStage::m_Max_Indx = 0;
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
CStage* CStage::Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, int nIndx)
{
	CStage* pBullet = new CStage;
	if (!pBullet)
		return nullptr;

	if (FAILED(pBullet->Init(pFilename, pos, rot, type, nIndx)))
	{
		delete pBullet;
		return nullptr;
	}
	m_nCntModel++;

	CSaveStage* pSave = CManager::GetSaveStage();
	pSave->AddModel(pBullet);

	return pBullet;
}
//===============
// 初期化処理
//===============
HRESULT CStage::Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type, int nIndx)
{
	m_pos = pos;
	m_rot = rot;
	m_type = type;
	m_nIndx = nIndx;

	if (m_Max_Indx < nIndx)
	{
		m_Max_Indx = nIndx;
	}
	
	// モデルファイル名を保存（安全な文字列コピー）
	strncpy(m_pFileName, pFilename, sizeof(m_pFileName));
	//モデル生成
	m_pModel = CModel::Create(m_pFileName, m_pos, m_rot);
	m_size = m_pModel->SetSize();
	
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