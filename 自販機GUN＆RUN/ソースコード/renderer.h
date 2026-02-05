//==============================
//
// レンダリング[renderer.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _RENDERER_H_
#define _RENDERER_H_

// インクルード
#include "main.h"

// レンダリングのクラス
class CRenderer
{
public:

	// メンバ関数
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 VecU);

	// 取得
	LPDIRECT3DDEVICE9 GetDevice(void) {return m_pD3DDevice;}
	LPDIRECT3DTEXTURE9 GetTexMT(void) { return m_pTexMT; }
private:	
	// メンバ変数
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	LPDIRECT3DTEXTURE9 m_pTexMT;
	LPDIRECT3DSURFACE9 m_pRendMT;
	LPDIRECT3DSURFACE9 m_pZBuffMT;
	D3DVIEWPORT9 m_viewportMT;
};

#endif