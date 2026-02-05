//===============================================
//
//  モデルゲージ処理[gaugeModel.cpp]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "gaugeModel.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//===============================================
// コンストラクタ
//===============================================
CGaugeModel::CGaugeModel(int nPriority) : CObjectX(nPriority)
{
	m_fBase = 0;
	m_pFrame = nullptr;
	m_pBack = nullptr;
}
//===============================================
// デストラクタ
//===============================================
CGaugeModel::~CGaugeModel()
{
}
//===============================================
// 生成処理
//===============================================
CGaugeModel* CGaugeModel::Create(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR3 size)
{
	CGaugeModel* pGause;
	//
	pGause = new CGaugeModel;
	//初期化
	if (FAILED(pGause->Init(pos, base, col, size)))
	{
		delete pGause;
		return nullptr;
	}

	return pGause;
}
//===============================================
// 初期化処理
//===============================================
HRESULT CGaugeModel::Init(D3DXVECTOR3 pos, float base, D3DXCOLOR col, D3DXVECTOR3 size)
{
	m_fBase = base;
	m_fMaxBase = base;
	m_col = col;
	m_size = size;
	
	CObjectX::Init("data\\MODEL\\gauge00.x", pos, VECTOR3_NULL);

	Set(m_fBase, m_col);

	return S_OK;
}
//===============================================
// 終了処理
//===============================================
void CGaugeModel::Uninit(void)
{
	CObjectX::Uninit();
}
//===============================================
// 更新処理
//===============================================
void CGaugeModel::Update(void)
{
	if (m_fBase <= 0.0f)
	{
		CObjectX::SetDraw(false);
	}
	else
	{
		CObjectX::SetDraw(true);
	}

	CObjectX::Update();
}
//===============================================
// 描画処理
//===============================================
void CGaugeModel::Draw(void)
{
	CObjectX::Draw();
}
//===============================================
// 設定
//===============================================
void CGaugeModel::Set(float fBase, D3DXCOLOR col)
{
	// 値を代入
	m_fBase = fBase;
	m_col = col;

	// レートを計算
	m_fRate = m_fBase / m_fMaxBase;
	m_fRate = max(0.0f, min(m_fRate, 1.0f));

	// 位置算出
	float fullHalfWidth = (m_size.x * m_fMaxBase) / 2.0f;
	float currentHalfWidth = fullHalfWidth * m_fRate;

	CObjectX::SetRealSize(D3DXVECTOR3(currentHalfWidth, m_size.y, m_size.z));	// サイズ調整
	CObjectX::SetColorChange(true);												// 色を変える
	CObjectX::SetColor(m_col);													// 色設定
	CObjectX::SetEmissive(true);												// 発光
}
//===============================================
// 振動設定
//===============================================
void CGaugeModel::SetShakeInfo(bool bShake, int nShakeTime, D3DXVECTOR3 MaxShake)
{
	SetShake(bShake, nShakeTime, MaxShake);

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetShake(bShake, nShakeTime, MaxShake);
	}
	if (m_pBack != nullptr)
	{
		m_pBack->SetShake(bShake, nShakeTime, MaxShake);
	}
}
//===============================================
// ゲージの枠を生成
//===============================================
void CGaugeModel::CreateFrame(const char* pFileName, D3DXCOLOR col)
{
	D3DXVECTOR3 pos = GetPos();			// 位置取得
	D3DXVECTOR3 size = m_size * 1.1f;	// サイズ取得
	size.x = m_size.x * 1.025f;			// サイズ調整

	// 枠生成
	m_pFrame = CObjectX::Create(pFileName, pos, VECTOR3_NULL, static_cast<int>(Priority::StageUI));
	// 色変え
	m_pFrame->SetColorChange(true);
	m_pFrame->SetColor(col);

	// 位置算出
	float fullHalfWidth = (size.x * m_fMaxBase) / 2.0f;
	float currentHalfWidth = fullHalfWidth * m_fRate;
	
	m_pFrame->SetRealSize(D3DXVECTOR3(currentHalfWidth, size.y, size.z));	// サイズ調整
	m_pFrame->CObjectX::SetEmissive(true);								// 発光
}
//===============================================
// ゲージの背景を生成
//===============================================
void CGaugeModel::CreateBack(const char* pFileName, D3DXCOLOR col)
{
	D3DXVECTOR3 size = m_size * 0.99f;	// サイズ調整

	// 背景生成
	m_pBack = CObjectX::Create(pFileName, D3DXVECTOR3(CObjectX::GetPos()), VECTOR3_NULL, static_cast<int>(Priority::StageUI) - 1);
	// 色変え
	m_pBack->SetColorChange(true);
	m_pBack->SetColor(col);

	// 位置算出
	float fullHalfWidth = (size.x * m_fMaxBase) / 2.0f;
	float currentHalfWidth = fullHalfWidth * m_fRate;
	
	m_pBack->SetRealSize(D3DXVECTOR3(currentHalfWidth, size.y, size.z));	// サイズ調整
	m_pBack->CObjectX::SetEmissive(true);									// 発光
}