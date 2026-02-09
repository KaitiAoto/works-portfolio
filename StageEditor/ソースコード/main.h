//==================
//
// C++ [main.h]
// Author:kaiti
//
//==================
#ifndef MAIN_H_
#define MAIN_H_

#define DIRECTINPUT_VERSION (0x0800)

#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include<Windows.h>
#include<time.h>
#include "d3dx9.h"
#include <crtdbg.h>
#include <random>
#include <chrono>
#include "dinput.h"
#include "Xinput.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//マクロ定義
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "自販機"
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define MAX_WORD (1024)
#define _CRTDBG_MAP_ALLOC

#endif