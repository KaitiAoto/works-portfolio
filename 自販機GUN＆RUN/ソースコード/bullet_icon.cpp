//==============================
//
// 弾アイコン[bullet_icon.h]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "bullet_icon.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "player.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CBulletIcon::CBulletIcon(int nPriority):CObject2D(nPriority)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = false;
}
//================
// デストラクタ
//================
CBulletIcon::~CBulletIcon()
{
}
//===========
// 生成処理
//===========
CBulletIcon* CBulletIcon::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// 生成
	CBulletIcon* pBulletIcon = new CBulletIcon;
	//初期化
	if (FAILED(pBulletIcon->Init(pos, fWidth, fHeight)))
	{// NULLチェック
		delete pBulletIcon;
		return nullptr;
	}
	return pBulletIcon;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBulletIcon::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//値を代入
	m_pos = pos;

	// メンバ変数を初期化
	m_bUse = false;

	// オブジェクト2Dの初期化
	CObject2D::Init("data\\TEXTURE\\bullet00.png", pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), fWidth, fHeight);

	// アイコンを切り替え
	ChangeIcon(CBullet::TYPE_CAN);

	// オブジェクトの種類設定
	SetObjType(TYPE_RESTOCK);

	return S_OK;
}
//============
// 終了処理
//============
void CBulletIcon::Uninit(void)
{
	// オブジェクト2Dの終了処理
	CObject2D::Uninit();
}
//============
// 更新処理
//============
void CBulletIcon::Update(void)
{	
	// 中身があれば
	if (m_nCntContens > 0)
	{
		m_bUse = true;		// 使用している状態にする
		// テクスチャ切り替え
		ChangeIcon(m_type);
	}
	// 中身がなければ
	else if (m_nCntContens <= 0)
	{
		m_bUse = false;		// 使用していない状態にする
	}
}
//============
// 描画処理
//============
void CBulletIcon::Draw(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// オブジェクト2Dの描画
		CObject2D::Draw();
	}
}
//==================
// アイコン切り替え
//==================
void CBulletIcon::ChangeIcon(CBullet::TYPE type)
{
	const char* pTexName = {};	// テクスチャ名格納用変数

	// 種類別にテクスチャ名を指定
	switch (type)
	{
	case CBullet::TYPE_CAN:
		pTexName = "data\\TEXTURE\\bullet00.png";
		break;
	case CBullet::TYPE_ICE:
		pTexName = "data\\TEXTURE\\bullet01.png";
		break;
	case CBullet::TYPE_CAPSULE:
		pTexName = "data\\TEXTURE\\bullet02.png";
		break;
	case CBullet::TYPE_PETBOTTLE:
		pTexName = "data\\TEXTURE\\bullet03.png";
		break;
	case CBullet::TYPE_DUST:
		pTexName = "data\\TEXTURE\\bullet04.png";
		break;
	case CBullet::TYPE_SNACK:
		pTexName = "data\\TEXTURE\\bullet05.png";
		break;
	case CBullet::TYPE_CIGARET:
		pTexName = "data\\TEXTURE\\bullet06.png";
		break;
	case CBullet::TYPE_CARD:
		pTexName = "data\\TEXTURE\\card01.jpg";
		break;
	case CBullet::TYPE_BOTTLE:
		pTexName = "data\\TEXTURE\\bullet08.png";
		break;
	default:
		break;
	}

	// テクスチャ割り当て
	CTexture* pTex = CManager::GetTex();	// テクスチャのポインタ取得
	int nIdxTex = pTex->Register(pTexName);	// テクスチャ割り当て
	CObject2D::SetIdxTex(nIdxTex);			// オブジェクト2Dのインデックス番号を設定
}
