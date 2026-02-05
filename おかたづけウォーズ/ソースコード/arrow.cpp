//===============================================
//
//  モデル矢印処理[arrow.cpp]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "arrow.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CArrow::CArrow(int nPriority) : CObjectX(nPriority)
{
}
//===============================================
// デストラクタ
//===============================================
CArrow::~CArrow()
{
}
//===============================================
// 生成処理
//===============================================
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size)
{
	CArrow* pGause;
	//
	pGause = new CArrow;
	//初期化
	if (FAILED(pGause->Init(pos, col, size)))
	{
		delete pGause;
		return nullptr;
	}

	return pGause;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CArrow::Init(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size)
{
	m_BasePos = pos;
	m_col = col;
	m_size = size;
	
	CObjectX::Init("data\\MODEL\\arrow00.x", pos, VECTOR3_NULL);

	CObjectX::SetColorChange(true);
	CObjectX::SetColor(m_col);
	CObjectX::SetEmissive(true);
	CObjectX::SetScale(m_size);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CArrow::Uninit(void)
{
	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CArrow::Update(void)
{
	static const float fUpSpeed = 1.5f;		// 上昇速度
	static const float fRotSpeed = 0.05f;	// 回転速度
	static const float fMaxPos = 50.0f;		// 最大移動値

	D3DXVECTOR3 pos = CObjectX::GetPos();	// 位置取得
	D3DXVECTOR3 rot = CObjectX::GetRot();	// 角度取得

	// 上昇するなら
	if (m_bUp == true)
	{
		// 上昇
		pos.y += fUpSpeed;
		// 最大値まで移動したら
		if (pos.y >= fMaxPos + m_BasePos.y)
		{
			// 上昇しない
			m_bUp = false;
		}
	}
	// それ以外
	else
	{
		// 下降
		pos.y -= fUpSpeed;
		// 最大値まで移動したら
		if (pos.y <= -fMaxPos + m_BasePos.y)
		{
			// 上昇する
			m_bUp = true;
		}
	}

	// 角度を少し回転
	rot.y += fRotSpeed;

	CObjectX::Set(pos, rot);
}
//===============================================
// 描画処理
//===============================================
void CArrow::Draw(void)
{
	CObjectX::Draw();
}