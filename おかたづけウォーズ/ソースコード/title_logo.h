//===============================================
//
// タイトルロゴ[title_logo.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _TITLE_LOGO_H_
#define _TITLE_LOGO_H_

// インクルード
#include "main.h"
#include "object2D.h"

// タイトルロゴクラス
class CTitleLogo:public CObject2D
{
public:

	// メンバ関数
	CTitleLogo(int nPriority = static_cast<int>(Priority::UI_Front));
	~CTitleLogo();
	HRESULT Init(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetStop(void) { return m_bStop; }

	// 静的メンバ関数
	static CTitleLogo* Create(const char* pTexName, D3DXVECTOR3 pos, float fWidth, float fHeight);

private:

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 位置
	D3DXVECTOR3 m_posStay;	// 止まる位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_move;		// 移動量
	bool m_bUse;			// 使用しているか
	bool m_bStop;			// 止まっているか
	int m_nTime;			// 止まっている時間
	int m_AnimCnt;			// アニメカウント
};

#endif