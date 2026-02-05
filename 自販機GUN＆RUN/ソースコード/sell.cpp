//==============================
//
// 相性[matchup.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "sell.h"
#include "manager.h"
#include "camera.h"

//
int CSell::m_nCntSellTime = 0;
int CSell::m_nCntSell = 0;

//==================
// コンストラクタ
//==================
CSell::CSell()
{
}
//================
// デストラクタ
//================
CSell::~CSell()
{

}
//
//
//
void CSell::Init(void)
{
	m_nCntSellTime = 0;
	m_nCntSell = 0;
}
//
//
//
void CSell::Update(void)
{
	if (CGame::GetMode() == CGame::MODE_PLAY)
	{
		CScoreMana* pTotalScore = CGame::GetTotalScore();

		static int nDecreaseTime = 60;

		if (CGame::GetTime()->GetTime() >= GAME_TIME / 6)
		{
			nDecreaseTime = 45;
		}

		m_nCntSellTime++;

		if (m_nCntSellTime >= nDecreaseTime)
		{
			int nNumBase = CEnemyBase::GetNum();
			int nTarget = rand() % nNumBase;

			int nCntBase = 0;

			CObject* pObj = CObject::GetTop(PRIORITY_ENEMYBASE);
			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();
				if (pObj->GetObjType() == CObject::TYPE_ENEMY_BASE)
				{
					if (nTarget == nCntBase)
					{
						CEnemyBase* pBase = (CEnemyBase*)pObj;
						if (pBase->GetUse() == true)
						{
							m_nCntSell++;
							pBase->SoldOut();
							break;
						}
					}
					
					pTotalScore->AddScore(1);

					nCntBase++;
				}
				pObj = pObjNext;
			}

			m_nCntSellTime = 0;
		}
	}
	else
	{
		m_nCntSellTime = 0;
	}

	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("売れた数：%d", m_nCntSell);
	pDegub->Print("拠点数：%d", CEnemyBase::GetNum());

}
