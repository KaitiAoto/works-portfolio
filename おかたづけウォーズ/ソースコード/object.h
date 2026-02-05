//===============================================
//
// オブジェクト[object.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECT_H_
#define _OBJECT_H_

// インクルード
#include "main.h"

// オブジェクトクラス
class CObject
{
public:
	// 描画順
	enum class Priority : int
	{
		None = 0,
		Map = 1,
		Field = 2,
		StageModel = 3,
		StageUI = 4,
		Character = 5,
		Effect = 6,
		UI = 7,
		UI_Back = 8,
		UI_Front = 9,
		Pause = 10,
		Max = 11
	};
	// 種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_BOSS,
		TYPE_STAGE,
		TYPE_FLOWERVASE,
		TYPE_EFFECT,
		TYPE_INTERACT,
		TYPE_PHYSICS,
		TYPE_MESH,
		TYPE_DAMEGEFIELD,
		TYPE_UI,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CObject(int nPriority = static_cast<int>(Priority::None));
	// 仮想関数
	virtual ~CObject();
	// 純粋仮想関数
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// 設定
	void SetObjType(TYPE type) { m_Objtype = type; }
	void SetPrio(int nPrio) { m_nPriority = nPrio; }
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
	static CObject* m_apTop[static_cast<int>(Priority::Max)];	// 先頭オブジェクトへのポインタ
	static CObject* m_apCur[static_cast<int>(Priority::Max)];	// 最後尾オブジェクトへのポインタ
	static int m_nNumAll;										// 最大数

};

#endif