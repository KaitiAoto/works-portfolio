//===============================================
//
//  モーション処理[motion.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _MOTION_H_
#define _MOTION_H_

// インクルード
#include "main.h"
#include "model.h"

// マクロ定義
#define MAX_PARTS (10)	// パーツの最大数
#define MAX_KEY (30)//キーの最大数

// モーションクラス
class CMotion
{
public:

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
	void StartBlend(int totype, int nBlendFrame = 10);
	D3DXVECTOR3 LerpVec3(const D3DXVECTOR3& a, const D3DXVECTOR3& b, float t);
	D3DXVECTOR3 LerpKey(int type, int part, int keyA, int keyB, float t, bool isPos);

	// 設定
	void SetType(int type);

	// 取得
	int GetType(void) { return m_type; }
	bool GetBlend(void) { return m_bBlend; }

	// 静的メンバ関数
	static CMotion* Load(const char* pFilename);

private:

	static constexpr int MAX_MOTION = 10;	// モーションの最大数

	// メンバ変数
	MOTION_INFO m_aMotionInfo[MAX_MOTION];	// 情報
	int m_type;								// 種類
	bool m_bLoop;							// ループするかどうか
	int m_nNumMotion;						// 総数
	int m_nKey;								// 現在のキーNo
	int m_nCntMotion;						// モーションのカウンター
	// モーションブレンド
	bool m_bBlend;							// ブレンド中か
	float m_fBlendRate;						// ブレンド進行
	int m_typeFrom;							// 元のモーション
	int m_typeTo;							// 移行先モーション
	int m_nBlendFrame;						// ブレンドにかけるフレーム数
	int m_nKeyFrom;							// 元のキー
	int m_nKeyTo;							// 移行先のきー
	int m_nCntFrom;							// 元のカウント
	int m_nCntTo;							// 移行先のカウント
};

#endif 