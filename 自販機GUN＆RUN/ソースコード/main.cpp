//=====================
//
// C++[main.cpp]
// Author : Kaiti Aoto
//
//=====================
#include "main.h"
#include "manager.h"
//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============
// メイン関数
//=============
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	//_CrtSetBreakAlloc(229);
	//_CrtSetBreakAlloc(1871);

	// メモリリーク検出用フラグの設定
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	CManager* pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowCursor(FALSE);
	bool bCursor = false;

	//マネージャーの生成
	pManager = new CManager;
	//マネージャーの初期化
	pManager->Init(hInstance, hWnd, TRUE);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	bool bFull = true;
#ifndef _DEBUG
	LONG style = GetWindowLong(hWnd, GWL_STYLE);
	style &= ~(WS_OVERLAPPEDWINDOW);
	style |= WS_POPUP;
	SetWindowLong(hWnd, GWL_STYLE, style);

	SetWindowPos(hWnd, HWND_TOP,
		0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
	bFull = true;
#endif

#ifdef _DEBUG
	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	bFull = false;

#endif

	UpdateWindow(hWnd);

	DWORD dwFrameCount; //フレームカウント
	DWORD dwFPSLastTime;//

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	int nCntFPS = 0;

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				nCntFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//マネージャー更新処理
				pManager->Update();

				if (pManager->GetInputKey()->GetTrigger(DIK_F4))
				{
					bCursor = !bCursor;
					ShowCursor(bCursor);
				}

				if (bCursor == false)
				{
					// ゲーム画面の中央にカーソルをセットする例
					int centerX = SCREEN_WIDTH / 2;
					int centerY = SCREEN_HEIGHT / 2;

					// クライアント領域の中央をスクリーン座標に変換
					POINT pt = { centerX, centerY };
					ClientToScreen(hWnd, &pt);

					// カーソル位置をセット
					SetCursorPos(pt.x, pt.y);
				}
				if (pManager->GetInputKey()->GetTrigger(DIK_F10))
				{
					if (bFull == false)
					{
						LONG style = GetWindowLong(hWnd, GWL_STYLE);
						style &= ~(WS_OVERLAPPEDWINDOW);
						style |= WS_POPUP;
						SetWindowLong(hWnd, GWL_STYLE, style);

						SetWindowPos(hWnd, HWND_TOP,
							0, 0,
							GetSystemMetrics(SM_CXSCREEN),
							GetSystemMetrics(SM_CYSCREEN),
							SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);

						bFull = true;
					}
					else
					{
						// ウィンドウモードに戻す
						LONG style = GetWindowLong(hWnd, GWL_STYLE);
						style &= ~(WS_POPUP);               // 枠なしを外す
						style |= WS_OVERLAPPEDWINDOW;       // 枠ありウィンドウ
						SetWindowLong(hWnd, GWL_STYLE, style);

						// ウィンドウサイズを戻す
						SetWindowPos(hWnd, HWND_TOP,
							CW_USEDEFAULT, CW_USEDEFAULT,
							rect.right - rect.left,
							rect.bottom - rect.top,
							SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);

						bFull = false;
					}
				}

				//マネージャー描画処理
				pManager->Draw();
				pManager->GetDebug()->Print("FPS：%d", nCntFPS);

				dwFrameCount++;

				Sleep(1);  // CPU負荷軽減
			}
		}
	}

	//マネージャーの破棄
	if (pManager != NULL)
	{
		//マネージャーの終了処理
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	ShowCursor(TRUE);

	timeEndPeriod(1);

	//ウインドウクラスの解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//=======================
//ウインドウプロシージャ
//=======================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]キーが押された場合
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
				break;
			}
			else
			{
				return 0;
			}
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}