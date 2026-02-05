//===============================================
//
// チュートリアル処理[tutorial.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// インクルード
#include "object.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "gatherArea.h"
#include "objectBillboard.h"
#include "arrow.h"
#include "interact.h"
#include "flowerVase.h"

// 前方宣言
class CScene;

// ゲームクラス
class CTutorial
{
public:
	// モード
	typedef enum
	{
		MODE_MOVE = 0,		// 移動
		MODE_THROW,			// 投げる
		MODE_DUST,			// ゴミ
		MODE_GOODS,			// 雑貨
		MODE_FLOWER,		// 花
		MODE_FLOWERVASE,	// 花瓶
		MODE_MISS,			// ミス
		MODE_MAX
	}MODE;
	// チュートリアルのチェック用関数を保持する構造体
	struct CheckItem
	{
		bool(CTutorial::* func)();   // メンバ関数ポインタ
	};

	// メンバ関数
	CTutorial();
	~CTutorial();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ResetInteract(CInteract* pInteract);

	// 設定
	void SetClear(bool bClear) { m_bClear = bClear; }
	// 取得
	MODE GetMode(void) { return m_mode; }

private:
	void CheckInit(void);

	bool Move(void);
	bool Throw(void);
	bool Dust(void);
	bool Goods(void);
	bool Flower(void);
	bool FlowerVase(void);
	bool Miss(void);

	// テクスチャ名
	static constexpr const char* TexName[MODE_MAX] =
	{
		"data\\TEXTURE\\tutorial00.jpg",
		"data\\TEXTURE\\tutorial01.jpg",
		"data\\TEXTURE\\tutorial02.jpg",
		"data\\TEXTURE\\tutorial03.jpg",
		"data\\TEXTURE\\tutorial04.jpg",
		"data\\TEXTURE\\tutorial04.jpg",
		"data\\TEXTURE\\tutorial05.jpg",
	};

	// メンバ変数
	MODE m_mode;								// モード
	bool m_bClear;								// クリアしたか
	CObject2D* m_pUI;							// UIへのポインタ
	CGatherArea* m_pArea;						// エリアへのポインタ
	CFlowerVase* m_pFlowerVase;					// 花瓶へのポインタ
	CObjectBillboard* m_pBillboard;				// ビルボードへのポインタ
	std::vector<CArrow*> m_pArrow;				// 矢印の動的配列
	std::vector<CInteract*> m_pInteract;		// インタラクトの動的配列
};

#endif