//===============================================
//
// ゲーム処理[game.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "sound.h"
#include "debugproc.h"
#include "pause.h"
#include "player.h"
#include "loadstage.h"
#include "boss.h"
#include "score.h"
#include "gatherArea.h"
#include "timerAnalog.h"
#include "tutorial.h"

// 前方宣言
class CScene;

// ゲームクラス
class CGame:public CScene
{
public:
	// モード
	typedef enum
	{
		MODE_NONE = 0,	// 通常
		MODE_PLAY,		// プレイ
		MODE_FIN,		// 終了
		MODE_PAUSE,		// ポーズ
		MODE_BACK,		// 戻る
		MODE_RESET,		// リセット
		MODE_MAX
	}MODE;
	// モード
	typedef enum
	{
		TYPE_TUTORIAL = 0,	// チュートリアル
		TYPE_GAME,	// 
		TYPE_MAX
	}TYPE;



	// メンバ関数
	CGame(TYPE mode);
	~CGame();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CalcScore(void);

	// 静的メンバ関数
	// 設定
	static void SetMode(MODE mode) { m_mode = mode; }
	static void SetPlayer(CPlayer* player) { m_pPlayer = player; }
	static void SetField(CObject3D* Field) { if (m_pField == nullptr) { m_pField = Field; } }

	// 取得
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CBoss* GetBoss(void) { return m_pBoss; }
	static CObject3D* GetField(void) { return m_pField; }
	static CPause* GetPause(void) { return m_pPause; }
	static CAbalogTimer* GetTimer(void) { return m_pTimer; }
	static CTutorial* GetTutorial(void) { return m_pTutorial; }
	static MODE GetMode(void) { return m_mode; }
	static TYPE GetType(void) { return m_type; }

private:

	static constexpr int GAME_TIME = 120;

	// 静的メンバ変数
	static CPlayer* m_pPlayer;			// プレイヤーへのポインタ
	static CLoadStage* m_pStage;		// ステージ読み込みへのポインタ
	static CObject3D* m_pField;			// フィールドへのポインタ
	static CBoss* m_pBoss;				// ボスへのポインタ
	static CAbalogTimer* m_pTimer;		// タイマーへのポインタ
	static CTutorial* m_pTutorial;		// チュートリアルへのポインタ
	static CPause* m_pPause;			// ポーズへのポインタ
	static MODE m_mode;					// ゲームモード
	static TYPE m_type;					// ゲームモード

};

#endif