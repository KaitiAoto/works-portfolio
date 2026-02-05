//===============================================
//
// 物理的なオブジェクト処理[objectPhysics.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECTPHYSICS_H_
#define _OBJECTPHYSICS_H_

// インクルード
#include "objectX.h"

// 物理的なオブジェクトクラス
class CObjectPhysics :public CObjectX
{
public:

	CObjectPhysics(int nPriority = static_cast<int>(Priority::StageModel));
	~CObjectPhysics();
	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 取得
	D3DXVECTOR3 GetMove(void) { return m_move; }
	bool GetBlow(void) { return m_bBlow; }
	// 設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void MovePos(D3DXVECTOR3 move) { m_pos += move; }
	void SetBlow(bool bThrow, D3DXVECTOR3 move) { m_bBlow = true; m_bThrow = bThrow; m_blowMove = move; }
	void SetUseColl(bool bUseColl) { m_bUseColl = bUseColl; }
	// 静的メンバ関数
	static CObjectPhysics* Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWeight);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	bool CollPlayer(void);
	bool CollSame(void);
	void Blow();

	static constexpr float CollRadius = 200.0f;	// 当たり判定検知半径

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_size;		// サイズ
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_blowMove;	// 吹き飛び移動量
	D3DXVECTOR3 m_posOld;	// 前回の位置
	bool m_bBlow;			// 吹き飛ぶか
	bool m_bThrow;			// 投げられたか
	bool m_bUseColl;		// 当たり判定をとるか
	float m_fWeight;		// 重さ

	// 静的メンバ変数
	static int m_nNumAll;	// 総数
};

#endif