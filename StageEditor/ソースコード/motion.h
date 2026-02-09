//==============================
//
//  モーション処理[motion.h]
//  Author:kaiti
//
//==============================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#include "model.h"

#define MAX_KEY (300)//キーの最大数

//クラス
class CMotion
{
public:
	//モーションの種類
	typedef enum
	{
		TYPE_NEUTRAL = 0,
		TYPE_MOVE,
		TYPE_ACTION,
		TYPE_JUMP,
		TYPE_LANDING,

		TYPE_MAX,
	}TYPE;

	//キーの構造体
	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;

		float fRotX;
		float fRotY;
		float fRotZ;
	}KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PARTS];
	}KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[MAX_KEY];
	}MOTION_INFO;

	//メンバ関数
	CMotion();
	//仮想関数
	~CMotion();

	void Update(const int nMaxPart, CModel** pModel);

	static CMotion* Load(const char* pFilename, CModel** model);

	void SetType(TYPE type) { m_type = type; }
private:
	MOTION_INFO m_aMotionInfo[TYPE_MAX];//モーション情報
	int m_nNumMotion;					//モーションの総数
	TYPE m_type;						//モーションの種類
	bool m_bLoop;						//ループするかどうか
	int m_nKey;							//現在のキーNo
	int m_nCntMotion;					//モーションのカウンター


};

#endif 