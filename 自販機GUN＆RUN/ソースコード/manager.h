//================================
//
// マネージャー処理[manager.h]
// Author : Kaiti Aoto
//
//================================

// 二重インクルード防止
#ifndef _MANAGER_H_
#define _MANAGER_H_

// インクルード
#include "main.h"
#include "scene.h"
#include "game.h"
#include "fade.h"
#include "result.h"
#include "ranking.h"

// マクロ定義
#define GRAVITY (0.7f)

// 前方宣言
class CRenderer;
class CInputKey;
class CInputMouse;
class CInputPad;
class CCamera;
class CTexture;
class CSound;
class CLight;
class CDebugProc;
class CScene;

// マネージャークラス
class CManager
{
public:

	// メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// 取得
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CInputKey* GetInputKey(void) { return m_pInputKey; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CInputPad* GetInputPad(void) { return m_pInputPad; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTex(void) { return m_pTex; }
	static CSound* GetSound(void) { return m_pSound; }
	static CDebugProc* GetDebug(void) { return m_pDebug; }
	static CFade* GetFade(void) { return m_pFade; }
	static CScene* GetScene(void) { return m_pScene; }
	
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); };
	static bool GetPause(void) { return m_bPause; }

	// 設定
	static void SetMode(CScene::MODE mode);
	static void SetPause(bool bPause) { m_bPause = bPause; }

private:
	//メンバ関数
	HRESULT CreatePointa(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void ReleasePointa(void);

	//静的メンバ変数
	static CRenderer* m_pRenderer;			// レンダラーへのポインタ
	static CInputKey* m_pInputKey;			// キー入力へのポインタ
	static CInputMouse* m_pInputMouse;		// マウス入力へのポインタ
	static CInputPad* m_pInputPad;			// パッド入力へのポインタ
	static CTexture* m_pTex;				// テクスチャへのポインタ
	static CCamera* m_pCamera;				// カメラへのポインタ
	static CLight* m_pLight;				// ライトへのポインタ
	static CSound* m_pSound;				// サウンドへのポインタ
	static CDebugProc* m_pDebug;			// デバッグへのポインタ
	static CFade* m_pFade;					// フェードへのポインタ
	static CScene* m_pScene;				// シーンへのポインタ

	static bool m_bPause;					// ポーズ中か

};

#endif