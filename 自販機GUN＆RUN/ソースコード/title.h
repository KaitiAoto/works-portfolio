//================================
//
// タイトル処理[title.h]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _TITLE_H_
#define _TITLE_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "title_logo.h"

// 前方宣言
class CScene;
class CLoadStage;

// タイトルクラス
class CTitle:public CScene
{
public:
	// メンバ関数
	CTitle();
	~CTitle();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTitleLogo* GetLogo(void) { return m_pLogo; }

private:

	// 静的メンバ変数
	static CLoadStage* m_pStage;	// ステージへのポインタ
	static CTitleLogo* m_pLogo;		//ロゴへのポインタ
};

#endif