//====================================
//
// プリコンパイルヘッダー [pch.h]
// Author : Kaiti Aoto
//
//====================================

// 二重インクルード防止
#ifndef _PCH_H_
#define _PCH_H_

#define DIRECTINPUT_VERSION (0x0800)

// インクルード
#include <stdio.h>
#include <string.h>
#include<Windows.h>
#include<time.h>
#include "d3dx9.h"
#include <crtdbg.h>
#include <random>
#include <chrono>
#include "dinput.h"
#include "Xinput.h"

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

// マクロ定義
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "自販機GUN＆RUN"
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define MAX_WORD (1024)
#define _CRTDBG_MAP_ALLOC
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)//座標・法線・カラー・テクスチャ

// 頂点情報(2D)の構造体
typedef struct
{
	D3DXVECTOR3 pos;	 // 頂点座標
	float rhw;			 // 法線ベクトル
	D3DCOLOR col;		 // 頂点カラー
	D3DXVECTOR2 tex;	 // テクスチャ座標
}VERTEX_2D;

// 頂点情報(3D)の構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;

#endif