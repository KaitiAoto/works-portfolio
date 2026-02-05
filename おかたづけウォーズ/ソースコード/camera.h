//===============================================
//
//　カメラ処理[camera.h]
//　Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _CAMERA_H_
#define _CAMERA_H_

// インクルード
#include "main.h"

// カメラクラス
class CCamera
{
public:

	// メンバ関数
	CCamera();
	// 仮想関数
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateMenu(void);

	// 取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	float GetDis(void) { return m_fDistance; }
	// 設定
	void Set(void);
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }
	void SetShake(float fShakeX, float fShakeY, int nTime);
private:
	void UpdateShake(void);

	// メンバ変数
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_posVDest;		// 視点
	D3DXVECTOR3 m_posRDest;		// 注視点
	D3DXVECTOR3 m_vecU;			// 上方向ベクトル
	D3DXVECTOR3 m_posVOffSet;	// 視点
	D3DXVECTOR3 m_rot;			// 向き
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	float m_fDistance;			// 視点から注視点の距離
	float m_fMaxShakeX;			// 最大X振動
	float m_fMaxShakeY;			// 最大Y振動
	int m_nShakeTime;			// 振動時間
	bool m_bShake;				// 振動するかどうか
	bool m_bMove;				// 移動するか

};

#endif