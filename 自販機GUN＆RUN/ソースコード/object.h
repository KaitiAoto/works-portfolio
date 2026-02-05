//==============================
//
// オブジェクト[object.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _OBJECT_H_
#define _OBJECT_H_

// インクルード
#include "main.h"

// マクロ定義
#define NUM_PRIORITY (11)

#define PRIORITY_PLAYER (5)
#define PRIORITY_BULLET (5)
#define PRIORITY_ENEMY (5)
#define PRIORITY_ENEMYBASE (5)
#define PRIORITY_VENDER (5)
#define PRIORITY_STAGE (6)
#define PRIORITY_GIMMIC (5)
#define PRIORITY_ITEM (5)
#define PRIORITY_START (4)

// オブジェクトクラス
class CObject
{
public:

	// 種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_ENEMY_BASE,
		TYPE_BULLET,
		TYPE_VENDER,
		TYPE_GIMMICK,
		TYPE_SHADOW,
		TYPE_STAGE,
		TYPE_FIELD,
		TYPE_WALL,
		TYPE_START,
		TYPE_EFFECT,
		TYPE_ITEM,
		TYPE_NUMBER,
		TYPE_SCORE,
		TYPE_TIME,
		TYPE_RESTOCK,
		TYPE_BULLETCNT,
		TYPE_GAUGE,
		TYPE_UI,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CObject(int nPriority = 3);
	// 仮想関数
	virtual ~CObject();

	// 純粋仮想関数
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// 設定
	void SetObjType(TYPE type) { m_Objtype = type; }
	// 取得
	TYPE GetObjType(void) { return m_Objtype; }
	int GetPrio(void) { return m_nPriority; }
	CObject* GetNext(void) { return m_pNext; }

	// 静的メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject* GetTop(int nPrio) { return m_apTop[nPrio];}

protected:
	void Release(void);

private:
	static void DeathCheck(void);

	// メンバ変数
	CObject* m_pPrev;						// 前のオブジェクトへのポインタ
	CObject* m_pNext;						// 次のオブジェクトへのポインタ
	TYPE m_Objtype;							// 種類
	int m_nPriority;						// 描画順
	bool m_bDeath;							// 死亡フラグ

	// 静的メンバ変数
	static CObject* m_apTop[NUM_PRIORITY];	// 先頭オブジェクトへのポインタ
	static CObject* m_apCur[NUM_PRIORITY];	// 最後尾オブジェクトへのポインタ
	static int m_nNumAll;					// 最大数

};

#endif