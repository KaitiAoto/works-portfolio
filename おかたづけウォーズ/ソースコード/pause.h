//===============================================
//
// ポーズ[pause.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _PAUSE_H_
#define _PAUSE_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object2D.h"
#include "pause_manager.h"

// ポーズクラス
class CPause
{
public:
	// メンバ関数
	CPause();
	~CPause();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 静的メンバ関数
	static CPause* Create();
	static void SetDraw(bool bDraw) { m_bDraw = bDraw; }
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点情報

	// 静的メンバ変数
	static CPauseMana* m_pPauseMana;	// ポーズマネージャーへのポインタ
	static bool m_bDraw;				// 描画するかどうか
};

#endif