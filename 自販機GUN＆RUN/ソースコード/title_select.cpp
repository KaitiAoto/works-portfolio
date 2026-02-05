//===================================
//
// タイトル選択[title_select.cpp]
// Author : Kaiti Aoto
//
//====================================
#include "title_select.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"
#include "title_logo.h"
#include "title.h"
#include "easing.h"
//==================
// コンストラクタ
//==================
CTitleSelect::CTitleSelect(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Select = SELECT_GAME;

	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		m_fAnim[nCnt] = 0.0f;
	}
}
//================
// デストラクタ
//================
CTitleSelect::~CTitleSelect()
{
}
//===========
// 生成処理
//===========
CTitleSelect* CTitleSelect::Create(D3DXVECTOR3 pos)
{
	CTitleSelect* pObject2D = nullptr;
	pObject2D = new CTitleSelect;
	if (pObject2D != nullptr)
	{
		//初期化
		pObject2D->Init(pos);
	}
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CTitleSelect::Init(D3DXVECTOR3 pos)
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//値を代入
	m_pos = pos;

	m_pCursor = CObject2D::Create("data\\TEXTURE\\select00.png", m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), T_SELECT_X * 1.25f, T_SELECT_Y * 1.25f, 7);

	m_pSelect[SELECT_GAME] = CObject2D::Create("data\\TEXTURE\\start00.png", m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), T_SELECT_X, T_SELECT_Y, 8);
	m_pSelect[SELECT_END] = CObject2D::Create("data\\TEXTURE\\end00.png", D3DXVECTOR3(m_pos.x, m_pos.y + T_SELECT_Y, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), T_SELECT_X, T_SELECT_Y, 8);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CTitleSelect::Uninit(void)
{

	CObject::Release();
}
//============
// 更新処理
//============
void CTitleSelect::Update(void)
{
	const float fAnim = 0.05f;
	Select();

	m_pCursor->SetPos(m_pSelect[m_Select]->GetPos());

	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_Select == nCnt)
		{
			m_pSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

			// 進行度を増やす（最大 1.0）
			m_fAnim[nCnt] += fAnim;
			if (m_fAnim[nCnt] > 1.0f)
			{
				m_fAnim[nCnt] = 1.0f;
			}
		}
		else
		{
			m_pSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			// 進行度を減らす（最小 0.0）
			m_fAnim[nCnt] -= fAnim;
			if (m_fAnim[nCnt] < 0.0f)
			{
				m_fAnim[nCnt] = 0.0f;
			}
		}
		// OutExpo でスケール計算
		float eased = CEasing::OutExpo(m_fAnim[nCnt]);
		float scale = 1.0f + 0.25f * eased;
		m_pSelect[nCnt]->SetSize(T_SELECT_X * scale, T_SELECT_Y * scale);
	}
}
//============
// 描画処理
//============
void CTitleSelect::Draw(void)
{
}
//
//
//
void CTitleSelect::Select(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	//パッド
	CInputPad* pPad = CManager::GetInputPad();

	CSound* pSound = CManager::GetSound();

	CFade* pFade = CManager::GetFade();

	int wheel = 0;
	wheel =	pInputMouse->GetWheel();

	CDebugProc* pDegub = CManager::GetDebug();
	pDegub->Print("wheel：%d", wheel);

	if (pInputKey->GetTrigger(DIK_W) == true || pPad->GetStickTriggerUp() == true || wheel > 0 || pPad->GetTrigger(CInputPad::JOYKEY_UP) == true)
	{
		m_Select--;
		if (m_Select < 0) m_Select = SELECT_MAX - 1;

		pSound->PlaySound(CSound::SOUND_LABEL_SELECT);
	}
	else if (pInputKey->GetTrigger(DIK_S) == true || pPad->GetStickTriggerDown() == true || wheel < 0 || pPad->GetTrigger(CInputPad::JOYKEY_DOWN) == true)
	{
		m_Select++;
		if (m_Select >= SELECT_MAX) m_Select = SELECT_GAME;

		pSound->PlaySound(CSound::SOUND_LABEL_SELECT);
	}

	if (CTitle::GetLogo()->GetStop() == true)
	{
		if (pInputMouse->GetTrigger(0) == true || pPad->GetTrigger(CInputPad::JOYKEY_A) == true)
		{
			switch (m_Select)
			{
			case SELECT_GAME:
				pFade->Set(CScene::MODE_GAME);
				break;
			case SELECT_END:
				PostQuitMessage(0);
				break;
			default:
				break;
			}

			pSound->PlaySound(CSound::SOUND_LABEL_CONFIRM);
		}
	}
}
