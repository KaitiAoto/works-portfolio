//===============================================
//
// タイマー[timer.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "timer.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "debugproc.h"

//===============================================
// コンストラクタ
//===============================================
CTimer::CTimer(int nPriority) :CObject(nPriority)
{

}
//===============================================
// デストラクタ
//===============================================
CTimer::~CTimer()
{
}
//===============================================
// 生成処理
//===============================================
CTimer* CTimer::Create(D3DXVECTOR3 pos, bool bLess)
{
	CTimer* pTimer = new CTimer;

	pTimer->m_bLess = bLess;

	pTimer->Init(pos, TIMER_SIZE, TIMER_SIZE);


	return pTimer;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CTimer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 分タイム
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_pMinute[nCnt] = CNumber::Create("data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (nCnt * TIMER_SIZE * 2.5f), pos.y, 0.0f), fWidth, fHeight);
	}

	// 位置を少しずらす
	pos.x += (2.5 * TIMER_SIZE * 2.5f);

	// コロン生成
	CObject2D::Create("data\\TEXTURE\\colon00.png", D3DXVECTOR3(pos.x - (TIMER_SIZE * 1.75f), pos.y, 0.0f), { 0.0f,0.0f,0.0f }, D3DXVECTOR2(TIMER_SIZE * 1.5f, TIMER_SIZE * 1.5f), 8);

	// 秒タイム
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_pSecond[nCnt] = CNumber::Create("data\\TEXTURE\\number001.png", D3DXVECTOR3(pos.x + (nCnt * TIMER_SIZE * 2.5f), pos.y, 0.0f), fWidth, fHeight);
	}

	// 縦横幅設定
	m_fWidth = TIMER_SIZE;
	m_fHeight = TIMER_SIZE;

	m_nTimer = 0;	// タイマーをクリア

	if (m_bLess == true)
	{
		// 時間設定
		CTimer::AddTime(120);
	}
	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CTimer::Uninit(void)
{
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		// 分破棄
		m_pMinute[nCnt]->Uninit();
		delete m_pMinute[nCnt];
		m_pMinute[nCnt] = nullptr;
		// 秒破棄
		m_pSecond[nCnt]->Uninit();
		delete m_pSecond[nCnt];
		m_pSecond[nCnt] = nullptr;
	}

	CObject::Release();
}
//===============================================
// 更新処理
//===============================================
void CTimer::Update(void)
{
	// 減少式なら
	if (m_bLess == true)
	{
		m_nCntTime++;
		if (m_nTimer <= 10)
		{
			CntDown();
		}
		if (m_nCntTime >= 60)
		{
			m_nCntTime = 0;
			// 1秒減らす
			CTimer::AddTime(-1);

			m_fWidth = TIMER_SIZE;
			m_fHeight = TIMER_SIZE;
		}

		if (m_nTimer <= 0)
		{
			m_nTimer = 0;
			CGame::SetMode(CGame::MODE_FIN);
		}
	}

	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("分：秒＝%d,%d", m_nDrawMinute, m_nDrawSecond);

}
//===============================================
// カウントダウン
//===============================================
void CTimer::CntDown(void)
{
	static D3DXCOLOR col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	m_fWidth += 2.0f;
	m_fHeight += 2.0f;

	if (m_nCntTime >= 60)
	{
		col.a = 1.0f;
	}
	col.a -= 0.01f;


	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_pSecond[nCnt] != nullptr)
		{
			CNumber* pNumber = m_pSecond[nCnt];
			pNumber->SetSize(m_fWidth, m_fHeight);
			pNumber->SetColor(col);
		}

		if (m_pMinute[nCnt] != nullptr)
		{
			CNumber* pNumber = m_pMinute[nCnt];
			pNumber->SetSize(m_fWidth, m_fHeight);
			pNumber->SetColor(col);
		}
	}
}
//===============================================
// 描画処理
//===============================================
void CTimer::Draw(void)
{
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_pMinute[nCnt]->Draw();

		m_pSecond[nCnt]->Draw();
	}
}
//===============================================
// 時間加算
//===============================================
void CTimer::AddTime(int nAdd)
{
	m_nTimer += nAdd;

	if (m_nTimer <= 0)
	{
		m_nTimer = 0;
	}

	m_nDrawMinute = m_nTimer / 60;
	m_nDrawSecond = m_nTimer % 60;

	Second();
	Minute();
}
//===============================================
// 時間設定
//===============================================
void CTimer::SetTime(int nTime)
{
	m_nTimer = nTime;
	AddTime(0);
}
//===============================================
// 秒単位のテクスチャ切り替え
//===============================================
void CTimer::Second(void)
{
	int nData = TIMERDATA * 10;
	int nData2 = TIMERDATA;
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_pSecond[nCnt] != nullptr)
		{
			int aPosTexU;

			CNumber* pNumber = m_pSecond[nCnt];

			aPosTexU = m_nDrawSecond % nData / nData2;
			nData /= 10;
			nData2 /= 10;

			pNumber->SetTex((aPosTexU * 0.1f), (aPosTexU * 0.1f) + 0.1f, 0.0f, 1.0f);
		}
	}
}
//===============================================
// 分単位のテクスチャ切り替え
//===============================================
void CTimer::Minute(void)
{
	int nData = TIMERDATA * 10;
	int nData2 = TIMERDATA;
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_pMinute[nCnt] != nullptr)
		{
			int aPosTexU;

			CNumber* pNumber = m_pMinute[nCnt];

			aPosTexU = m_nDrawMinute % nData / nData2;
			nData /= 10;
			nData2 /= 10;

			pNumber->SetTex((aPosTexU * 0.1f), (aPosTexU * 0.1f) + 0.1f, 0.0f, 1.0f);
		}
	}
}