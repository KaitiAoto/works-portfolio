//===================
//
// スコア[score.h]
// Author : Kaiti Aoto
//
//===================
#include "score.h"
#include "renderer.h"
#include "manager.h"
//静的メンバ変数
int CScore::m_nPriority = 0;

//==================
// コンストラクタ
//==================
CScore::CScore(int nPriority) :CObject(nPriority)
{
	m_nPriority = nPriority;
	m_nIdxTex = 0;
	m_pNumber = nullptr;
}
//================
// デストラクタ
//================
CScore::~CScore()
{
}
//===========
// 生成処理
//===========
CScore* CScore::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CScore* pScore = new CScore;
	//初期化
	pScore->Init(pos, fWidth, fHeight);

	return pScore;
}
void CScore::SetColor(D3DXCOLOR col)
{
	m_pNumber->SetColor(col);
}
//===============
// 初期化処理
//===============
HRESULT CScore::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//初期化
	m_pNumber = CNumber::Create("data\\TEXTURE\\number002.png", pos, fWidth, fHeight);
	
	SetObjType(TYPE_SCORE);

	return S_OK;
}
//============
// 終了処理
//============
void CScore::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		delete m_pNumber;
		m_pNumber = nullptr;
	}
	CObject::Release();
}
//============
// 更新処理
//============
void CScore::Update(void)
{

}
//============
// 描画処理
//============
void CScore::Draw(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Draw();
	}
}