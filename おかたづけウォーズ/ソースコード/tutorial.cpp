//===============================================
//
// チュートリアル処理[tutorial.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "tutorial.h"
#include "manager.h"
#include "fade.h"
#include "gatherAreaManager.h"
#include "file.h"
#include "objectX.h"

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial()
{
	// 値をクリア
	m_mode = MODE_MOVE;
	m_bClear = false;
	m_pUI = nullptr;
	m_pFlowerVase = nullptr;
}
//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{
}
//===============================================
// 初期化
//===============================================
void CTutorial::Init(void)
{
	// UI生成
	m_pUI = CObject2D::Create(TexName[0], D3DXVECTOR3(1100.0f, 150.0f, 0.0f), VECTOR3_NULL, D3DXVECTOR2(250.0f, 200.0f));

	// ゲームBGM
	CSound* pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_GAME);
}
//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	// インタラクトの動的配列をクリア
	m_pInteract.clear();
}
//===============================================
// 指定インタラクトを配列から削除
//===============================================
void CTutorial::ResetInteract(CInteract* pInteract)
{
	auto it = std::find(m_pInteract.begin(), m_pInteract.end(), pInteract);
	if (it != m_pInteract.end())
	{
		m_pInteract.erase(it);
	}
}
//===============================================
// 更新
//===============================================
void CTutorial::Update(void)
{
	// 上から順にチェックする関数のリスト
	static const CheckItem CheckList[] =
	{
		{ &CTutorial::Move},		// 移動
		{ &CTutorial::Throw},		// 投げ
		{ &CTutorial::Dust},		// ゴミ
		{ &CTutorial::Goods},		// 雑貨
		{ &CTutorial::Flower},		// 花
		{ &CTutorial::FlowerVase},	// 花瓶
		{ &CTutorial::Miss},		// ミス
	};

	// 登録されている全てのチェック処理を調べる
	auto& item = CheckList[m_mode];
	//item.func が指すチェックする関数を呼ぶ
	m_bClear = (this->*item.func)();
	// クリアしているなら
	if (m_bClear == true)
	{
		m_mode = static_cast<MODE>(m_mode + 1);

		// モードが最大まで終わったら
		if (m_mode >= MODE_MAX)
		{
			// チュートリアル終了
			CGame::SetMode(CGame::MODE_FIN);
		}
		else
		{
			// 初期設定
			CheckInit();
		}
		return;
	}
}
//===============================================
// 描画
//===============================================
void CTutorial::Draw(void)
{

}
//===============================================
// 調べる種類ごとの初期設定
//===============================================
void CTutorial::CheckInit(void)
{
	// クリアしていない状態にする
	m_bClear = false;

	//-------------------------------------------
	// 動的配列をクリア
	//-------------------------------------------
	// 全矢印破棄
	for (auto* pArrow : m_pArrow)
	{
		// NULLチェック
		if (pArrow != nullptr)
		{
			pArrow->Uninit();	// 終了処理
			pArrow = nullptr;	// NULLにする
		}
	}
	m_pArrow.clear();	// ポインタのリストをすべて削除

	// 全インタラクト破棄
	for (auto* pInteract : m_pInteract)
	{
		// NULLチェック
		if (pInteract != nullptr)
		{
			pInteract = nullptr;	// NULLにする
		}
	}
	m_pInteract.clear();	// ポインタのリストをすべて削除

	if (m_pBillboard != nullptr)
	{
		m_pBillboard->SetDraw(false);
		m_pBillboard->Uninit();
		m_pBillboard = nullptr;
	}

	// UIのテクスチャ変更
	CTexture* pTex = CManager::GetTex();
	m_pUI->SetIdxTex(pTex->Register(TexName[m_mode]));

	switch (m_mode)
	{
		//---------------------------------------
		// 投げについて
		//---------------------------------------
	case MODE_THROW:
	{
		// ゴミ生成
		CInteract* pInteract = CInteract::Create(D3DXVECTOR3(0.0f, 500.0f, 0.0f), VECTOR3_NULL, CInteract::TYPE_DUST);
		m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する

		break;
	}
		//---------------------------------------
		// ゴミについて
		//---------------------------------------
	case MODE_DUST:
	{
		// 位置
		D3DXVECTOR3 pos = D3DXVECTOR3(-50.0f, 500.0f, 0.0f);
		// ゴミ生成
		for (int nCnt = 0; nCnt < CInteractDust::TYPE_MAX; nCnt++)
		{
			CInteract* pInteract = CInteractDust::Create(pos, VECTOR3_NULL, CInteract::TYPE_DUST, (CInteractDust::TYPE)nCnt);
			m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する

			// 位置を少しずらす
			pos.x += 100.0f;
		}

		// 灰生成
		CInteract* pInteract = CInteractDust::Create(pos, VECTOR3_NULL, CInteract::TYPE_DUST);
		pInteract->ChangeAsh();
		m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する


		// 矢印の位置
		static D3DXVECTOR3 ArrowPos[4] =
		{
			{ 800.0f, 0.0f, 0.0f },
			{ -800.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 800.0f },
			{ 0.0f, 0.0f, -500.0f },
		};

		// ４か所に矢印設置
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			CArrow* arrow = CArrow::Create(ArrowPos[nCnt], D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
			m_pArrow.push_back(arrow);	// 配列の末尾に arrow のポインタを追加する
		}
		break;
	}

		//---------------------------------------
		// 雑貨について
		//---------------------------------------
	case MODE_GOODS:
	{
		// 位置
		D3DXVECTOR3 pos = D3DXVECTOR3(-100.0f, 500.0f, 0.0f);
		// 雑貨生成
		for (int nCnt = 0; nCnt < CInteractGoods::TYPE_MAX; nCnt++)
		{
			CInteract* pInteract = CInteractGoods::Create(pos, VECTOR3_NULL, CInteract::TYPE_GOODS, (CInteractGoods::TYPE)nCnt);
			m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する

			// 位置を少しずらす
			pos.x += 100.0f;
		}

		// エリア生成
		m_pArea = CGatherArea::Create(D3DXVECTOR3(300.0f, 0.0f, 300.0f), VECTOR3_NULL, 150.0f, 25.0f);

		// 矢印設置
		CArrow* arrow = CArrow::Create(D3DXVECTOR3(300.0f, 100.0f, 400.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		m_pArrow.push_back(arrow);	// 配列の末尾に arrow のポインタを追加する

		break;
	}

		//---------------------------------------
		// 花について
		//---------------------------------------
	case MODE_FLOWER:
	{
		// 矢印の位置
		static D3DXVECTOR3 FlowerPos[5] =
		{
			{ 0.0f, 500.0f, 0.0f },
			{ 300.0f, 500.0f, 300.0f },
			{ -300.0f, 500.0f, 300.0f },
			{ 300.0f, 500.0f, -300.0f },
			{ -300.0f, 500.0f, -300.0f },
		};

		// 花生成
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			CInteract* pInteract = CInteract::Create(FlowerPos[nCnt], VECTOR3_NULL, CInteract::TYPE_FLOWER);
			m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する
		}
		break;
	}

		//---------------------------------------
		// 花瓶について
		//---------------------------------------
	case MODE_FLOWERVASE:
	{
		// 花瓶の位置
		D3DXVECTOR3 FlowerPos = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
		// 花生成
		m_pFlowerVase = CFlowerVase::Create(FlowerPos, VECTOR3_NULL);

		// 位置を少しずらす
		FlowerPos.y += 200.0f;	
		// 矢印設置
		CArrow* arrow = CArrow::Create(FlowerPos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		m_pArrow.push_back(arrow);	// 配列の末尾に arrow のポインタを追加する

		FlowerPos.x += 150.0f;
		m_pBillboard = CObjectBillboard::Create("data\\TEXTURE\\tutorial06.jpg", FlowerPos, VECTOR3_NULL, 120.0f, 80.0f, 5);

		break;
	}
		//---------------------------------------
		// ミスについて
		//---------------------------------------
	case MODE_MISS:
	{
		// 雑貨生成
		CInteract* pInteract = CInteract::Create(D3DXVECTOR3(0.0f, 500.0f, 0.0f), VECTOR3_NULL, CInteract::TYPE_GOODS);
		m_pInteract.push_back(pInteract);	// 配列の末尾に Interact のポインタを追加する

		// 矢印の位置
		static D3DXVECTOR3 ArrowPos[4] =
		{
			{ 800.0f, 0.0f, 0.0f },
			{ -800.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 800.0f },
			{ 0.0f, 0.0f, -500.0f },
		};

		// ４か所に矢印設置
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			CArrow* arrow = CArrow::Create(ArrowPos[nCnt], D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
			m_pArrow.push_back(arrow);	// 配列の末尾に arrow のポインタを追加する
		}
		
		break;
	}
	default:
		break;
	}
}
//===============================================
// 移動について
//===============================================
bool CTutorial::Move(void)
{
	static int nMoveTime = 0;

	// 移動していれば
	if (CGame::GetPlayer()->GetbMove() == true)
	{
		// カウンター加算
		nMoveTime++;
	}

	// 一定時間移動したら
	if (nMoveTime >= 60)
	{
		nMoveTime = 0;		// カウンターを０に
		m_bClear = true;	// クリア
	}

	return m_bClear;
}
//===============================================
// 投げについて
//===============================================
bool CTutorial::Throw(void)
{
	// クリアしていない状態でインタラクトがなくなったら
	if (m_bClear == false && CInteract::GetNumDust() <= 0)
	{
		m_pInteract.push_back(CInteract::Create(D3DXVECTOR3(0.0f, 500.0f, 0.0f), VECTOR3_NULL, CInteract::TYPE_DUST));
	}

	return m_bClear;
}
//===============================================
// ゴミについて
//===============================================
bool CTutorial::Dust(void)
{
	// クリアしていない状態でインタラクトがなくなったら
	if (m_bClear == false && CInteract::GetNumDust() <= 0)
	{
		m_bClear = true;	// クリア
	}

	return m_bClear;
}
//===============================================
// 雑貨について
//===============================================
bool CTutorial::Goods(void)
{
	// クリアしていない状態でインタラクトがなくなったら
	if (m_bClear == false && CInteract::GetNumGoods() < CInteractGoods::TYPE_MAX)
	{
		m_pInteract.push_back(CInteract::Create(D3DXVECTOR3(0.0f, 500.0f, 0.0f), VECTOR3_NULL, CInteract::TYPE_GOODS));
	}

	// エリアに入っていれば
	if (CGatherArea::GetGoodsIn() >= CInteractGoods::TYPE_MAX && !m_pInteract.empty())
	{
		// エリア上昇
		m_pArea->SetRising(true);

		// 全インタラクト分回す
		for (auto* pInteract : m_pInteract)
		{
			// インタラクト上昇
			if (pInteract->GetRising() == false)
			{
				pInteract->SetMyRising();
			}
		}
	}

	return m_bClear;
}
//===============================================
// 花について
//===============================================
bool CTutorial::Flower(void)
{
	// 花束を持っていれば
	if (CGame::GetPlayer()->GetHaveBouquet() == true)
	{
		m_bClear = true;	// クリア
	}

	// クリアしていない状態でインタラクトがなくなったら
	if (m_bClear == false && CInteract::GetNumFlower() <= 0)
	{
		m_pInteract.push_back(CInteract::Create(D3DXVECTOR3(300.0f, 500.0f, 300.0f), VECTOR3_NULL, CInteract::TYPE_FLOWER));
	}

	return m_bClear;
}
//===============================================
// 花瓶について
//===============================================
bool CTutorial::FlowerVase(void)
{
	// 花瓶に花があれば
	if (m_pFlowerVase->GetHaveBouquet() == true)
	{
		m_bClear = true;	// クリア
	}

	return m_bClear;
}
//===============================================
// ミスについて
//===============================================
bool CTutorial::Miss(void)
{
	// クリアしていない状態でインタラクトがなくなったら
	if (m_bClear == false && CInteract::GetNumGoods() <= 0)
	{
		m_pInteract.push_back(CInteract::Create(D3DXVECTOR3(0.0f, 500.0f, 0.0f), VECTOR3_NULL, CInteract::TYPE_GOODS));
	}

	return m_bClear;
}
