//===============================================
//
// ステージモデル[stage.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "stage.h"
#include "renderer.h"
#include "manager.h"

//===============================================
// コンストラクタ
//===============================================
CStage::CStage(int nPriority):CObjectX(nPriority)
{
}
//===============================================
// デストラクタ
//===============================================
CStage::~CStage()
{

}
//===============================================
// 生成処理
//===============================================
CStage* CStage::Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, bool bShadow, TYPE type)
{
	CStage* pStage = new CStage;

	if (FAILED(pStage->Init(pFilename, pos, rot, bShadow, type)))
	{
		delete pStage;
		return nullptr;
	}

	return pStage;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CStage::Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, bool bShadow, TYPE type)
{
	m_type = type;

	//モデル生成
	CObjectX::Init(pFilename, pos, rot);
	D3DXVECTOR3 size = CObjectX::GetSize();

	if (bShadow == true)
	{
		CShadow::Create(pos, rot, size.x, size.z, CShadow::TYPE_SQUARE);
	}
	//オブジェクトの種類設定
	SetObjType(TYPE_STAGE);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CStage::Uninit(void)
{
	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CStage::Update(void)
{
	CObjectX::Update();
}
//===============================================
// 描画処理
//===============================================
void CStage::Draw(void)
{
	CObjectX::Draw();
}
//===============================================
// 全て振動させる
//===============================================
void CStage::ShakeAll(void)
{
	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageUI));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();
		CObject::TYPE type = pObj->GetObjType();

		// 種類が一致していれば
		if (type == CObject::TYPE_STAGE)
		{
			// ステージモデル情報取得
			CStage* pStage = (CStage*)pObj;

			// 振動設定
			pStage->SetShake(true, 30, D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
		pObj = pObjNext;
	}
}