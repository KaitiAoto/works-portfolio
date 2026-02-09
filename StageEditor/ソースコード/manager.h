//================================
//
// マネージャー処理[manager.h]
// Author:kaiti
//
//================================
#ifndef MANAGER_H_
#define MANAGER_H_

#include "main.h"

//マクロ定義
#define GRAVITY (0.2f)

//前方宣言
class CRenderer;
class CInputKey;
class CInputMouse;
class CPlayer;
class CCamera;
class CTexture;
class CObject3D;
class CLight;
class CCollision;
class CDebugProc;
class CLoadStage;
class CModelSet;
class CSaveStage;

//マネージャークラス
class CManager
{
public:

	//メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CInputKey* GetInputKey(void) { return m_pInputKey; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CTexture* GetTex(void) { return m_pTex; }
	static CObject3D* GetObj3D(void) { return m_pObj3D; }
	//static CRestock* GetRestock(void) { return m_pRestock; }
	static CCollision* GetColl(void) { return m_pColl; }
	static CDebugProc* GetDebug(void) { return m_pDebug; }
	static CModelSet* GetModelSet(void) { return m_pModelSet; }
	static CSaveStage* GetSaveStage(void) { return m_pSaveStage; }

private:
	//メンバ関数
	void CreateAll(void);
	HRESULT CreatePointa(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void ReleasePointa(void);

	//静的メンバ変数
	static CRenderer* m_pRenderer;				//レンダラーへのポインタ
	static CInputKey* m_pInputKey;				//キー入力へのポインタ
	static CInputMouse* m_pInputMouse;		// マウス入力へのポインタ
	static CPlayer* m_pPlayer;					//パッドへのポインタ
	static CTexture* m_pTex;					//テクスチャへのポインタ
	static CCamera* m_pCamera;					//カメラへのポインタ
	static CLight* m_pLight;					//ライトへのポインタ
	static CObject3D* m_pObj3D;					//3Dオブジェクトへのポインタ
	static CCollision* m_pColl;					//当たり判定へのポインタ
	static CDebugProc* m_pDebug;				//デバッグへのポインタ
	static CLoadStage* m_pStage;
	static CSaveStage* m_pSaveStage;
	static CModelSet* m_pModelSet;

};

#endif