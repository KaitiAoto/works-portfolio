//============================
//
// 矢印[arrow.cpp]
// Author : Kaiti Aoto
//
//============================

// インクルード
#include "arrow.h"
#include "game.h"

//==================
// コンストラクタ
//==================
CArrow::CArrow(int nPriority) :CObject3D(nPriority)
{
	// 値をクリア
	m_bUse = true;
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_TargetPos = { 0.0f,0.0f,0.0f };
}
//================
// デストラクタ
//================
CArrow::~CArrow()
{
}
//==============
// 生成処理
//==============
CArrow* CArrow::Create(D3DXVECTOR3 pos)
{
	// 生成
	CArrow* pArrow = new CArrow;
	// 初期化
	if (FAILED(pArrow->Init(pos)))
	{// NULLチェック
		delete pArrow;
		return nullptr;
	}
	return pArrow;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CArrow::Init(D3DXVECTOR3 pos)
{
	const float fWidth = 25.0f;		// 横の長さ
	const float fHeight = 25.0f;	// 縦の長さ

	// 値を代入
	m_pos = pos;

	// メンバ変数を初期化
	m_bUse = true;
	m_TargetPos = CGame::GetStart()->GetPos();

	// 3Dオブジェクトの初期化
	CObject3D::Init("data\\TEXTURE\\arrow00.png", pos, { 0.0f, 0.0f, 0.0f }, fWidth, fHeight, CObject3D::TYPE_SHADOW);

	// オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CArrow::Uninit(void)
{
	// 3Dオブジェクトの終了処理
	CObject3D::Uninit();
}
//============
// 更新処理
//============
void CArrow::Update(void)
{
	//--------------
	// 向き
	//--------------
	// 方向ベクトル
	D3DXVECTOR3 toTarget = m_TargetPos - m_pos;
	// 正規化
	D3DXVec3Normalize(&toTarget, &toTarget);
	float angle = atan2f(toTarget.x, toTarget.z);
	m_rot.y = angle - D3DX_PI / 2;	// 値を代入

	//向きを設定
	CObject3D::SetRot(m_rot);

	//---------------
	// 位置
	//---------------
	// プレイヤー取得
	CPlayer* pPlayer = CGame::GetPlayer();
	m_pos = pPlayer->GetPos();// プレイヤーの位置を代入
	// 位置を設定
	CObject3D::SetPos({ m_pos.x, m_pos.y + (pPlayer->GetSize().y * 1.5f), m_pos.z });

	// ゲームのモードがPLAYなら
	if (CGame::GetMode() == CGame::MODE_PLAY)
	{
		m_bUse = false;	// 使わなくする
	}
	// 使われていなければ
	if (m_bUse == false)
	{
		// 終了処理
		Uninit();
	}
}
//============
// 描画処理
//============
void CArrow::Draw(void)
{
	// 使っていたら
	if (m_bUse == true)
	{
		CObject3D::Draw(); // 3Dオブジェクトの描画
	}
}