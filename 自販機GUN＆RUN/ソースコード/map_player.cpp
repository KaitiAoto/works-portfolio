//==============================
//
// 補充処理[restock.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "map_player.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CMapPlayer::CMapPlayer(int nPriority):CObject2D(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nIdxTex = 0;
	m_bUse = true;
}
//================
// デストラクタ
//================
CMapPlayer::~CMapPlayer()
{
}
//===========
// 生成処理
//===========
CMapPlayer* CMapPlayer::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CMapPlayer* pObject2D;
	pObject2D = new CMapPlayer;
	//初期化
	pObject2D->Init(pos, fWidth, fHeight);
	return pObject2D;
}
//===============
// 初期化処理
//===============
HRESULT CMapPlayer::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//値を代入
	m_pos = pos;
	m_bUse = true;

	CObject2D::Init("data\\TEXTURE\\playerIcon00.png", pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight);

	m_pArrow = CObject2D::Create("data\\TEXTURE\\arrow01.png", D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight, 9);

	//オブジェクトの種類設定
	SetObjType(TYPE_UI);

	return S_OK;
}
//============
// 終了処理
//============
void CMapPlayer::Uninit(void)
{
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CMapPlayer::Update(void)
{
	PlayerMove();

	ArrowMove();
}
//============
// 描画処理
//============
void CMapPlayer::Draw(void)
{
	if (m_bUse == true)
	{
		CObject2D::Draw();
	}
}
//============================
// プレイヤーのアイコン移動
//============================
void CMapPlayer::PlayerMove(void)
{
	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();

	// プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = pPlayer->GetPos();

	// ミニマップの情報
	const float miniMapCenterX = CGame::GetMap()->GetPos().x;
	const float miniMapCenterY = CGame::GetMap()->GetPos().y;
	const float miniMapW = CGame::GetMap()->GetWidth();
	const float miniMapH = CGame::GetMap()->GetHeight();

	// ステージのサイズ
	const float worldSizeX = 1900.0f;
	const float worldSizeZ = 1900.0f;

	float normX = (playerPos.x + worldSizeX / 2.0f) / worldSizeX;
	float normZ = (playerPos.z + worldSizeZ / 2.0f) / worldSizeZ;

	// 中心を基準に描画
	float iconX = miniMapCenterX - miniMapW / 2.0f + normX * miniMapW;
	float iconY = miniMapCenterY - miniMapH / 2.0f + (1.0f - normZ) * miniMapH;

	m_pos = D3DXVECTOR3(iconX, iconY, 0.0f);

	CObject2D::SetPos(m_pos);
}
//==============
// 矢印移動
//==============
void CMapPlayer::ArrowMove(void)
{
	//プレイヤー情報取得
	CPlayer* pPlayer = CGame::GetPlayer();
	// プレイヤーの位置を取得
	float fPlayerRotY = pPlayer->GetRot().y - (D3DX_PI / 2);

	float fRadius = 15.0f;

	float fArrowX = -cosf(fPlayerRotY)* fRadius;
	float fArrowY = -sinf(fPlayerRotY) * fRadius;

	D3DXVECTOR3 ArrowRot = D3DXVECTOR3(0.0f, 00.0f, 0.0f);
	ArrowRot.z = -fPlayerRotY + (D3DX_PI / 2);

	m_pArrow->Set(D3DXVECTOR3(m_pos.x + fArrowX, m_pos.y + fArrowY, m_pos.z), ArrowRot);

	m_pArrow->SetUse(m_bUse);
}
