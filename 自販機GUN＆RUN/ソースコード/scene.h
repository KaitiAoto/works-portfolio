//================================
//
// シーン処理[scene.h]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _SCENE_H_
#define _SCENE_H_

// インクルード
#include "main.h"
#include "object.h"

// シーンクリア
class CScene:public CObject
{
public:
	// モード
	typedef enum
	{
		MODE_TITLE=0,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,

		MODE_END,
		MODE_MAX
	}MODE;

	// メンバ関数
	CScene(MODE mode);
	// 純粋仮想関数
	virtual ~CScene() = 0;
	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// 取得
	MODE GetMode(void) { return  m_mode; }

	// 静的メンバ関数
	static CScene* Create(MODE mode);

private:
	// メンバ変数
	MODE m_mode;
};


#endif