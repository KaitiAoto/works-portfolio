//===============================================
//
// ポーズマネージャー[pause_manager.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "pause_manager.h"
#include "manager.h"
#include "texture.h"
#include "debugproc.h"
#include "input.h"

//===============================================
// コンストラクタ
//===============================================
CPauseMana::CPauseMana()
{
	// 値をクリア
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_Select = SELECT_CONTNUE;
	m_bDraw = false;
}
//===============================================
// デストラクタ
//===============================================
CPauseMana::~CPauseMana()
{
}
//===============================================
// 生成処理
//===============================================
CPauseMana* CPauseMana::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CPauseMana* pPauseMana = new CPauseMana;

	// 初期化
	if (FAILED(pPauseMana->Init(pos)))
	{
		delete pPauseMana;
		return nullptr;
	}

	return pPauseMana;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CPauseMana::Init(D3DXVECTOR3 pos)
{
	//値を代入
	m_pos = pos;

	// 背景生成
	m_pBack = CObject2D::Create("data\\TEXTURE\\gauge00.jpeg", pos, VECTOR3_NULL, D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), 10);
	m_pBack->SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f));

	// 選択し生成
	m_pSelect[SELECT_CONTNUE] = CPauseSelect::Create("data\\TEXTURE\\contnue00.png", D3DXVECTOR3(m_pos.x, m_pos.y + (P_SELECT_Y * SELECT_CONTNUE), m_pos.z), P_SELECT_X, P_SELECT_Y);	// 戻る
	m_pSelect[SELECT_RESTART] = CPauseSelect::Create("data\\TEXTURE\\restart00.png", D3DXVECTOR3(m_pos.x, m_pos.y + (P_SELECT_Y * SELECT_RESTART), m_pos.z), P_SELECT_X, P_SELECT_Y);	// やり直す
	m_pSelect[SELECT_END] = CPauseSelect::Create("data\\TEXTURE\\end00.png", D3DXVECTOR3(m_pos.x, m_pos.y + (P_SELECT_Y * SELECT_END), m_pos.z), P_SELECT_X, P_SELECT_Y);				// 終わる

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CPauseMana::Uninit(void)
{
}
//===============================================
// 更新処理
//===============================================
void CPauseMana::Update(void)
{
	// 選択
	Select();

	// 選択肢数分
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		// 選択肢を描画するか設定
		m_pSelect[nCnt]->SetDraw(m_bDraw);

		// 選択しているものなら
		if (m_Select == nCnt)
		{
			// 色を変える
			m_pSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		}
		// それ以外なら
		else
		{
			// 色を戻す
			m_pSelect[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	//パッド
	CInputPad* pPad = CManager::GetInputPad();

	// ポーズキーが押されたら
	if (pInputKey->GetTrigger(DIK_TAB) == true || pInputKey->GetTrigger(DIK_P) == true || pPad->GetTrigger(CInputPad::JOYKEY_START) == true)
	{
		// ポーズ解除
		CManager::SetPause(false);
	}
	// ポーズ中なら
	if (CManager::GetPause() == true)
	{
		// 背景を表示
		m_pBack->SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.5f));
	}
	// それ以外なら
	else
	{
		// 背景を透過
		m_pBack->SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f));
	}
}
//===============================================
// 描画処理
//===============================================
void CPauseMana::Draw(void)
{
}
//===============================================
// 選択肢処理
//===============================================
void CPauseMana::Select(void)
{
	// キー取得
	CInputKey* pInputKey = CManager::GetInputKey();
	// マウス取得
	CInputMouse* pInputMouse = CManager::GetInputMouse();
	// パッド
	CInputPad* pPad = CManager::GetInputPad();
	// 音情報取得
	CSound* pSound = CManager::GetSound();

	int wheel = 0;
	wheel =	pInputMouse->GetWheel();

	// 上下選択
	if (pInputKey->GetTrigger(DIK_W) == true || pPad->GetStickTriggerUp() == true || wheel > 0 || pPad->GetTrigger(CInputPad::JOYKEY_UP) == true)
	{
		// 選択を１つ上にする
		m_Select--;
		// ０以下なら一番下にする
		if (m_Select < 0) m_Select = SELECT_MAX - 1;

		// 音
		pSound->PlaySound(CSound::SOUND_LABEL_SELECT);
	}
	else if (pInputKey->GetTrigger(DIK_S) == true || pPad->GetStickTriggerDown() == true || wheel < 0 || pPad->GetTrigger(CInputPad::JOYKEY_DOWN) == true)
	{
		// 選択を１つ下にする
		m_Select++;
		// 最大以上なら一番上にする
		if (m_Select >= SELECT_MAX) m_Select = SELECT_CONTNUE;

		// 音
		pSound->PlaySound(CSound::SOUND_LABEL_SELECT);
	}

	// 決定
	if (pInputKey->GetTrigger(DIK_RETURN) == true || pInputMouse->GetTrigger(0) == true || pPad->GetTrigger(CInputPad::JOYKEY_A) == true)
	{
		// 選択肢ごとの処理
		switch (m_Select)
		{
			// 戻る
		case SELECT_CONTNUE:
			break;

			// やり直す
		case SELECT_RESTART:
			CGame::SetMode(CGame::MODE_RESET);
			break;

			// 終わり
		case SELECT_END:
			CGame::SetMode(CGame::MODE_BACK);
			break;
		default:
			break;
		}
		// ポーズ終了
		CManager::SetPause(false);

		// 音
		pSound->PlaySound(CSound::SOUND_LABEL_SELECT);
	}
}