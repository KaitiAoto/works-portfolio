//=====================================
//
// 敵マネージャー[enemy_manager.h]
// Author:kaiti
//
//=====================================
#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "main.h"
#include "enemy.h"

//#define ENEMY_SPAN (300)

//オブジェクト2Dクラス
class CEnemyMana
{
public:

	//メンバ関数
	CEnemyMana();
	~CEnemyMana();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	static CEnemyMana* Create();
	static void Add(void) { m_nCntEnemy++; }
	static void Sub(void) { m_nCntEnemy--; }
private:
	//メンバ変数
	static CEnemyMana* m_pEnemyMane;	//オブジェクトへのポインタ
	static int m_nCntEnemy;

	int m_nCntSpan;
};

#endif