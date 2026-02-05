//==============================
//
// バフアイコン[buff_icon.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _BUFF_ICON_H_
#define _BUFF_ICON_H_

// インクルード
#include "main.h"
#include "object2D.h"
#include "buff.h"

// マクロ定義
#define BUFF_X (300)
#define BUFF_Y (100)

// バフアイコンクラス
class CBuffIcon:public CObject2D
{
public:
	// 状態
	typedef enum
	{
		STATE_STAY = 0,
		STATE_SLIDEIN,
		STATE_SLIDEOUT,
		STATE_MAX
	}STATE;


	// メンバ関数
	CBuffIcon(int nPriority = 8);
	~CBuffIcon();

	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	bool GetUse(void){return m_bUse;}
	// 設定処理
	void SetUse(bool bUse) { m_bUse = bUse; }

	// 静的メンバ関数
	static CBuffIcon* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:

	//メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posStay;	// 待機位置
	D3DXVECTOR3 m_posOut;	// 画面外位置
	D3DXVECTOR3 m_rot;		// 角度

	bool m_bUse;			// 使用しているか
	bool m_bClear;			// クリアしているか
	int m_nIdxTex;			// テクスチャのインデックス番号
	CBuff::TYPE m_type;		// 種類
	STATE m_state;			// 状態
	const char* m_apFileName[CBuff::TYPE_MAX];// テクスチャ名
};

#endif