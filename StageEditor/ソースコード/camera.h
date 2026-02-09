//================================
//
//　カメラ処理[camera.h]
//　Author:kaiti
//
//================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"


//クラス
class CCamera
{
public:

	//メンバ関数
	CCamera();
	//仮想関数
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }

private:
	D3DXVECTOR3 m_posV;//視点
	D3DXVECTOR3 m_posR;//注視点
	D3DXVECTOR3 m_posVDest;//視点
	D3DXVECTOR3 m_posRDest;//注視点

	D3DXVECTOR3 m_vecU;//上方向ベクトル
	D3DXMATRIX m_mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;//ビューマトリックス
	D3DXVECTOR3 m_rot;//向き
	float m_fDistance;//視点から注視点の距離

	bool m_bUse;
};

#endif