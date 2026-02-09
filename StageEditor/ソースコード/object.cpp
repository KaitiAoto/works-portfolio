//==============================
//
// オブジェクト[object.cpp]
// Author:kaiti
//
//==============================
#include "object.h"
#include "manager.h"
#include "camera.h"
//静的メンバ変数
int CObject::m_nNumAll = 0;
CObject* CObject::m_apObject[NUM_PRIORITY][MAX_OBJECT] = {};
//==================
// コンストラクタ
//==================
CObject::CObject(int nPriority)
{
	m_nId = 0;
	m_Objtype = TYPE_NONE;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[nPriority][nCnt] == nullptr)
		{
			m_apObject[nPriority][nCnt] = this;
			m_nId = nCnt;			//ID保存
			m_nNumAll++;			//総数加算
			m_nPriority = nPriority;//描画順保存
			break;
		}
	}
}

//================
// デストラクタ
//================
CObject::~CObject()
{
}
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	return E_NOTIMPL;
}
//=======================
// 全オブジェクトの破棄
//=======================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				m_apObject[nCntPri][nCnt]->Uninit();
			}
		}
	}
}
//=======================
// 全オブジェクトの更新
//=======================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				m_apObject[nCntPri][nCnt]->Update();
			}
		}
	}
}
//=======================
// 全オブジェクトの描画
//=======================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();
	pCamera->Set();

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nCntPri][nCnt] != nullptr)
			{
				m_apObject[nCntPri][nCnt]->Draw();
			}
		}
	}
}
//=====================
// オブジェクトの破棄
//=====================
void CObject::Release(void)
{ 
	int nId = this->m_nId;
	int nPriority = this->m_nPriority;

	if (m_apObject[nPriority][nId] != nullptr)
	{
		delete m_apObject[nPriority][nId];
		m_apObject[nPriority][nId] = nullptr;

		m_nNumAll--;
	}
}