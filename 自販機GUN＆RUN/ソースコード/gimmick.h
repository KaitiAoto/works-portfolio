//==============================
//
// ギミック[gimmick.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"

// マクロ定義
#define MAX_DIS (300.0f)
#define GIMMICK_SPEED (8.0f)

// ギミッククラス
class CGimmick :public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_CAR = 0,
		TYPE_POLE,

		TYPE_MAX
	}TYPE;

	// メンバ関数
	CGimmick(int nPriority = PRIORITY_GIMMIC);
	~CGimmick();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetType(TYPE type) { m_type = type; };
	static TYPE SetType(const char* pFileName);

	// 取得
	TYPE GetType(void) { return m_type; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }

	// 静的メンバ関数
	static CGimmick* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);

private:
	void Move(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_posOffSet;	// 位置(オフセット)
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_move;			// 移動量
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_rotDest;		// 目標の向き
	D3DXVECTOR3 m_size;			// サイズ

	CModel* m_pModel;			// モデルへのポインタ
	TYPE m_type;				// 種類
	bool m_bUse;				// 使用しているか
	float m_fMoveDis;			// 移動量
	float m_fMaxDis;			// 最大移動量
};

#endif