//================================
//
// ゲーム処理[game.cpp]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _GAME_H_
#define _GAME_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "object.h"
#include "object2D.h"
#include "player.h"
#include "input.h"
#include "effect.h"
#include "score_manager.h"
#include "timer_manager.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "sound.h"
#include "meshfield.h"
#include "enemy_base.h"
#include "vender.h"
#include "bullet_counter_manager.h"
#include "restock.h"
#include "collision.h"
#include "debugproc.h"
#include "matchup.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "gauge_hp.h"
#include "loadstage.h"
#include "wall_invisible.h"
#include "gimmick.h"
#include "start.h"
#include "gauge_reset.h"
#include "pause.h"
#include "gauge_enemy.h"
#include "item.h"
#include "particle.h"
#include "tutorial.h"
#include "meshcircle.h"
#include "effect_circle.h"
#include "minimap.h"
#include "startUI.h"
#include "buff.h"
#include "screen_flash.h"
#include "enemy_group.h"
#include "sell.h"
#include "tutorial_conveni.h"

// マクロ定義
#define GAME_TIME (180)

// 前方宣言
class CScene;

// ゲームクラス
class CGame:public CScene
{
public:
	// モード
	typedef enum
	{
		MODE_NONE=0,
		MODE_PLAY,
		MODE_FIN,
		MODE_PAUSE,
		MODE_BACK,
		MODE_RESET,
		MODE_TUTORIAL,
		MODE_TUTORIAL_CONVENI,
		MODE_MAX
	}MODE;

	// メンバ関数
	CGame();
	~CGame();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// 設定
	static void SetPlayer(CPlayer* player) { m_pPlayer = player; }
	static void SetMode(MODE mode) { m_mode = mode; }
	static void SetTutoConveni(CTutorialConveni* Tutoconveni) { m_pTutoConveni = Tutoconveni; }

	// 取得
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CTimerMana* GetTime(void) { return m_pTimer; }
	static CObject3D* GetObj3D(void) { return m_pObj3D; }
	static CScoreMana* GetBreakCnt(void) { return m_pBreakCnt; }
	static CScoreMana* GetTotalScore(void) { return m_pTotalScore; }

	static CCollision* GetColl(void) { return m_pColl; }
	static CMatchUp* GetMatchup(void) { return m_pMatchup; }
	static CHpGauge* GetHpGauge(void) { return m_pHpGauge; }
	static CStart* GetStart(void) { return m_pStart; }
	static CResetGauge* GetReset(void) { return m_pReset; }
	static CMap* GetMap(void) { return m_pMap; }
	static CBuff* GetBuff(void) { return m_pBuff; }
	static CScreenFlash* GetFlash(void) { return m_pScreenFlash; }
	static CTutorial* GetTutorial(void) { return m_pTutorial; }
	static CPause* GetPause(void) { return m_pPause; }
	static CTutorialConveni* GetTutoConveni(void) { return m_pTutoConveni; }

	static MODE GetMode(void) { return m_mode; }

private:

	// 静的メンバ変数
	static CPlayer* m_pPlayer;					// プレイヤーへのポインタ
	static CTimerMana* m_pTimer;				// タイマーへのポインタ
	static CObject3D* m_pObj3D;					// 3Dオブジェクトへのポインタ
	static CScoreMana* m_pBreakCnt;				// 破壊スコアへのポインタ
	static CScoreMana* m_pTotalScore;			// トータルスコアへのポインタ

	static CCollision* m_pColl;					// 当たり判定へのポインタ
	static CMatchUp* m_pMatchup;				// タイプ相性へのポインタ
	static CHpGauge* m_pHpGauge;				// HPゲージへのポインタ
	static CLoadStage* m_pStage;				// ステージ読み込みへのポインタ
	static CStart* m_pStart;					// スタートへのポインタ
	static CResetGauge* m_pReset;				// リセットゲージへのポインタ
	static CMap* m_pMap;						// マップへのポインタ
	static CBuff* m_pBuff;						// バフへのポインタ
	static CScreenFlash* m_pScreenFlash;		// 画面点滅へのポインタ
	static CTutorial* m_pTutorial;				// チュートリアルへのポインタ
	static CPause* m_pPause;					// ポーズへのポインタ
	static CTutorialConveni* m_pTutoConveni;
	static CSell* m_pSell;
	static MODE m_mode;							// ゲームモード

};

#endif