//==============================
//
// オブジェクト[object.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "object.h"
#include "manager.h"
#include "camera.h"
//静的メンバ変数
int CObject::m_nNumAll = 0;
CObject* CObject::m_apTop[NUM_PRIORITY] = {};
CObject* CObject::m_apCur[NUM_PRIORITY] = {};

//==================
// コンストラクタ
//==================
CObject::CObject(int nPriority)
{
	if (m_apTop[nPriority] == NULL)
	{//自分が先頭なら
		m_apTop[nPriority] = this;
		m_pPrev = NULL;
	}
	else
	{//それ以外なら
		m_pPrev = m_apCur[nPriority];
		m_pPrev->m_pNext = this;
	}

	m_apCur[nPriority] = this;//自分を最後尾にする
	m_pNext = NULL;//自分の次クリア

	m_nNumAll++;			//総数加算
	m_nPriority = nPriority;//描画順保存
	m_bDeath = false;
}
//================
// デストラクタ
//================
CObject::~CObject()
{
}
//=======================
// 全オブジェクトの破棄
//=======================
void CObject::ReleaseAll(void)
{
	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = m_apTop[nCnt];
		while (pObj != NULL)
		{// pObjがＮＵＬＬじゃない
			CObject* pObjNext = pObj->m_pNext;//自身の次を保存
			
			pObj->Uninit();
			if (pObj->m_bDeath == true)
			{
				delete pObj;
				pObj = nullptr;
			}

			pObj = pObjNext;//次のオブジェクトを代入
		}
		//先頭と最後尾をクリア
		m_apTop[nCnt] = nullptr;
		m_apCur[nCnt] = nullptr;
	}
	m_nNumAll = 0;
}
//=======================
// 全オブジェクトの更新
//=======================
void CObject::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = m_apTop[nCnt];
		while (pObj != NULL)
		{// pObjがＮＵＬＬじゃない
			CObject* pObjNext = pObj->m_pNext;//自身の次を保存

			//更新			
			pObj->Update();
		
			pObj = pObjNext;//次のオブジェクトを代入
		}
	}

	DeathCheck();
}
//=======================
// 全オブジェクトの描画
//=======================
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();
	pCamera->Set();

	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("オブジェクト数：%d", m_nNumAll);

	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = m_apTop[nCnt];
		while (pObj != NULL)
		{// pObjがＮＵＬＬじゃない
			CObject* pObjNext = pObj->m_pNext;//自身の次を保存
			//描画
			pObj->Draw();

			pObj = pObjNext;//次のオブジェクトを代入
		}
	}
}
//=====================
// オブジェクトの破棄
//=====================
void CObject::Release(void)
{ 
	m_bDeath = true;
}
//===========================
// 死亡フラグチェック処理
//===========================
void CObject::DeathCheck(void)
{
	//死亡フラグチェック
	for (int nCnt = 0; nCnt < NUM_PRIORITY; nCnt++)
	{
		CObject* pObj = m_apTop[nCnt];
		while (pObj != NULL)
		{// pObjがＮＵＬＬじゃない
			CObject* pObjNext = pObj->m_pNext;//自身の次を保存

			if (pObj->m_bDeath == true)
			{
				if (pObj == m_apTop[nCnt])
				{//自分が先頭なら
					m_apTop[nCnt] = pObj->m_pNext;//先頭を自身の次(m_pNext)に
					if (m_apTop[nCnt])
					{
						m_apTop[nCnt]->m_pPrev = NULL;
					}
				}
				else if (pObj == m_apCur[nCnt])
				{//自分が最後尾なら
					m_apCur[nCnt] = pObj->m_pPrev;//最後尾を自身の前(m_pPrev)に
					if (m_apCur[nCnt])
					{
						m_apCur[nCnt]->m_pNext = NULL;//最後尾の次をNULLに
					}
				}
				else
				{//それ以外の中間地点なら
					pObj->m_pPrev->m_pNext = pObj->m_pNext;//自分の前の次のオブジェクト(this->m_pPrev->m_pNext)を自分の次(this->m_pNext)にする
					pObj->m_pNext->m_pPrev = pObj->m_pPrev;//自分の次の前のオブジェクト(this->m_pNext->m_pPrev)を自分の前(this->m_pPrev)にする
				}

				pObj->m_pNext = nullptr;
				pObj->m_pPrev = nullptr;

				m_nNumAll--;

				//削除
				delete pObj;
			}

			pObj = pObjNext;//次のオブジェクトを代入
		}
	}
}
