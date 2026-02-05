//===============================================
//
// 収集エリア[gatherArea.cpp]
// Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "gatherArea.h"
#include "interact.h"
#include "game.h"
#include "manager.h"
#include "meshcircle.h"

// 静的メンバ変数
int CGatherArea::m_nAllIn = 0;
int CGatherArea::m_nDustIn = 0;
int CGatherArea::m_nGoodsIn = 0;

//===============================================
// コンストラクタ
//===============================================
CGatherArea::CGatherArea(int nPriority) :CMeshCylinder(nPriority)
{
	// 値をクリア
	m_bBlink = false;
	m_bRiseReaction = true;
	m_bRising = false;
	m_pObon = nullptr;
}
//===============================================
// デストラクタ
//===============================================
CGatherArea::~CGatherArea()
{
}
//===============================================
// 生成処理
//===============================================
CGatherArea* CGatherArea::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fRadius, float fHeight)
{
	// インスタンス生成
	CGatherArea* pArea = new CGatherArea;

	// 初期化
	if (FAILED(pArea->Init(pos, rot, fRadius, fHeight)))
	{
		delete pArea;
		return nullptr;
	}

	return pArea;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CGatherArea::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fRadius, float fHeight)
{
	// 色を設定
	m_col = COLOR;

	// 親クラスの初期化
	CMeshCylinder::Init(pos, rot, fRadius, fHeight, m_col, CMeshCylinder::TYPE_BOTHSIDES);

	// エリア用モデル生成
	m_pObon = CObjectX::Create("data\\MODEL\\obon.x", pos, VECTOR3_NULL);

	// サイズを指定
	m_pObon->SetRealSize(D3DXVECTOR3(fRadius * 2.0f, 0.0f, fRadius * 2.0f));

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CGatherArea::Uninit(void)
{
	// 親クラスの終了
	CMeshCylinder::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CGatherArea::Update(void)
{
	// 上昇しなければ
	if (m_bRising == false)
	{
		// 演出
		Reaction();

		// 点滅するなら
		if (m_bBlink == true)
		{
			// 点滅
			Blink();
		}

		// インタラクトとの判定
		CollInteract();

		// 親クラスの更新
		CMeshCylinder::Update();
	}
	// 上昇するなら
	else if (m_bRising == true)
	{
		// 上昇
		Rising();
	}
}
//===============================================
// 描画処理
//===============================================
void CGatherArea::Draw(void)
{
	// 親クラスの描画
	CMeshCylinder::Draw();
}
//===============================================
// 常にする演出
//===============================================
void CGatherArea::Reaction(void)
{
	//---------------------------------
	// 回転
	//---------------------------------
	D3DXVECTOR3 rot = GetRot();	// 現在の角度取得
	rot.y += ROT_SPEED;			// 角度加算

	// 角度設定
	SetRot(rot);

	//---------------------------------
	// 上下動
	//---------------------------------
	D3DXVECTOR3 pos = GetPos();	// 現在の位置取得

	// 上昇するなら
	if (m_bRiseReaction == true)
	{
		pos.y += RISE_SPEED;	// 上昇
	}
	// 下降するなら
	else if (m_bRiseReaction == false)
	{
		pos.y -= RISE_SPEED;	// 下降
	}

	// 最大まで上がったら
	if (pos.y >= MAX_RISE)
	{
		m_bRiseReaction = false;	// 下降させる
	}
	// 最大まで下がったら
	else if (pos.y <= -MAX_RISE)
	{
		m_bRiseReaction = true;		// 上昇させる
	}

	// 位置設定
	SetPos(pos);
}
//===============================================
// 点滅
//===============================================
void CGatherArea::Blink(void)
{
	// アルファ値以外赤に変更
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, m_col.a);

	// アルファ値を下げる
	m_col.a -= 0.01f;

	// アルファ値が０以下になったら
	if (m_col.a <= 0.0f)
	{
		// 元に戻す
		m_col.a = COLOR.a;
	}

	// 色設定
	CMeshCylinder::SetColor(m_col);
}
//===============================================
// 上昇設定
//===============================================
void CGatherArea::SetRising(bool bRising, float fRiseSpeed, float fMaxRise)
{
	// 値を代入
	m_bRising = bRising;
	m_fRiseSpeed = fRiseSpeed;
	m_fMaxRise = fMaxRise;

	// 描画しない
	CMeshCylinder::SetUse(false);
}
//===============================================
// 上昇移動
//===============================================
void CGatherArea::Rising(void)
{
	D3DXVECTOR3 pos = m_pObon->GetPos();	// モデルの位置取得

	// 上昇
	pos.y += m_fRiseSpeed;

	// 上昇移動
	m_pObon->SetPos(pos);

	// 最大まで上昇したら
	if (pos.y >= m_fMaxRise)
	{
		// ゲーム中なら
		if (CGame::GetType() == CGame::TYPE_GAME)
		{
			// 花生成
			CInteract::Create(pos, VECTOR3_NULL, CInteract::TYPE_FLOWER);
		}

		// 終了
		m_pObon->Uninit();
		Uninit();
		return;
	}
}
//===============================================
// インタラクトとの判定
//===============================================
bool CGatherArea::CollInteract(void)
{
	// 総数カウント用変数
	int nAllIn = 0;
	int nDustIn = 0;
	int nGoodsIn = 0;

	// ステージモデルのみ調べる
	CObject* pObj = CObject::GetTop(static_cast<int>(Priority::StageModel));
	while (pObj != nullptr)
	{
		CObject* pObjNext = pObj->GetNext();		// 次のオブジェクト取得
		CObject::TYPE type = pObj->GetObjType();	// 種類を取得

		// 種類が一致していれば
		if (type == CObject::TYPE_INTERACT)
		{
			// 物理オブジェクト情報取得
			CInteract* pObjInteract = (CInteract*)pObj;
			D3DXVECTOR3 InteractPos = pObjInteract->GetPos();	// 位置
			D3DXVECTOR3 InteractSize = pObjInteract->GetSize();	// サイズ
			// 半径をサイズから算出
			float fRadius = max(InteractSize.x, max(InteractSize.y, InteractSize.z)) * 0.5f;

			// 半径での判定（エリア周辺にあるか）
			if (CManager::GetColl()->Radius(InteractPos, fRadius, GetPos(), GetRadius() * 1.75f) == true)
			{
				// 判定を取るようにする
				pObjInteract->SetUseColl(true);

				// 半径での判定（エリア範囲内にあるか）
				if (CManager::GetColl()->Radius(InteractPos, fRadius, GetPos(), GetRadius()) == true)
				{// 範囲内なら
					// 収集されていることにする
					pObjInteract->SetGather(true);

					// 範囲内カウント加算
					nAllIn++;

					// インタラクトのタイプ取得
					CInteract::TYPE InteractType = pObjInteract->GetType();

					// ゴミなら
					if (InteractType == CInteract::TYPE_DUST)
					{
						nDustIn++;	// ゴミとしてカウント
					}
					// 雑貨なら
					else if (InteractType == CInteract::TYPE_GOODS)
					{
						nGoodsIn++;	// 雑貨としてカウント
					}

					// 内側のオブジェクトのみ回収
					if (CManager::GetColl()->Radius(InteractPos, fRadius, GetPos(), GetRadius() * 0.65f) == true)
					{
						pObjInteract->SetDrop(false, GetPos());	// 零れ落ちない
					}
					else
					{
						pObjInteract->SetDrop(true, GetPos());	// 零れ落ちる
					}
				}
				else
				{
					// 収集されていることにする
					pObjInteract->SetGather(false);
				}
			}
			else
			{
				// 判定を取らないようにする
				pObjInteract->SetUseColl(false);
			}
		}
		// 次のオブジェクトへ
		pObj = pObjNext;
	}

	// 総数を代入
	m_nAllIn = nAllIn;
	m_nDustIn = nDustIn;
	m_nGoodsIn = nGoodsIn;

	return false;
}
