//==============================
//
//  モーション処理[motion.h]
//  Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _MOTION_H_
#define _MOTION_H_

// インクルード
#include "main.h"
#include "model.h"

// マクロ定義
#define MAX_KEY (300)//キーの最大数

// モーションクラス
class CMotion
{
public:
	// 種類
	typedef enum
	{
		TYPE_NEUTRAL = 0,
		TYPE_MOVE,
		TYPE_ACTION,
		TYPE_JUMP,
		TYPE_LANDING,

		TYPE_MAX,
	}TYPE;

	// キーの構造体
	typedef struct
	{
		float fPosX;	// X位置
		float fPosY;	// Y位置
		float fPosZ;	// Z位置

		float fRotX;	// X向き
		float fRotY;	// Y向き
		float fRotZ;	// Z向き
	}KEY;

	// キー情報の構造体
	typedef struct
	{
		int nFrame;				// フレーム数
		KEY aKey[MAX_PARTS];	//キーの構造体
	}KEY_INFO;

	// 構造体
	typedef struct
	{
		bool bLoop;					// ループするか
		int nNumKey;				// 最大キー数
		KEY_INFO aKeyInfo[MAX_KEY];	// キー情報の構造体
	}MOTION_INFO;

	// メンバ関数
	CMotion();
	// 仮想関数
	~CMotion();
	void Update(const int nMaxPart, CModel** pModel);
	void StartBlend(TYPE from, TYPE to, int nBlendFrame);
	D3DXVECTOR3 LerpVec3(const D3DXVECTOR3& a, const D3DXVECTOR3& b, float t);
	D3DXVECTOR3 LerpKey(TYPE type, int part, int keyA, int keyB, float t, bool isPos);

	// 設定
	void SetType(TYPE type) { m_type = type; }

	// 取得
	TYPE GetType(void) { return m_type; }

	// 静的メンバ関数
	static CMotion* Load(const char* pFilename);

private:
	// メンバ変数
	MOTION_INFO m_aMotionInfo[TYPE_MAX];// 情報
	TYPE m_type;						// 種類
	bool m_bLoop;						// ループするかどうか
	int m_nNumMotion;					// 総数
	int m_nKey;							// 現在のキーNo
	int m_nCntMotion;					// モーションのカウンター
	// モーションブレンド
	bool m_bBlend;						// ブレンド中か
	float m_fBlendRate;					// ブレンド進行 (0.0～1.0)
	TYPE m_typeFrom;					// 元のモーション
	TYPE m_typeTo;						// 移行先モーション
	int m_nBlendFrame;					// ブレンドにかけるフレーム数
	int m_nKeyFrom;						// 元のキー
	int m_nKeyTo;						// 移行先のきー
	int m_nCntFrom;						// 元のカウント
	int m_nCntTo;						// 移行先のカウント
};

#endif 