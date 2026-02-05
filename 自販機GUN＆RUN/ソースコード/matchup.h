//==============================
//
// 相性[matchup.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef MATCHUP_H_
#define MATCHUP_H_

// インクルード
#include "main.h"
#include "bullet.h"
#include "enemy.h"

// マクロ定義
#define ADVANTAGE (3) 
#define DISADVANTAGE (2)

// 相性クラス
class CMatchUp
{
public:
	// メンバ関数
	CMatchUp();
	~CMatchUp();

	int Check(CBullet::TYPE Bullet,CEnemy::TYPE Enemy);
	void One_Shot(void) { m_nAttack = ENEMY_LIFE; }
	void Advantage(void) { m_nAttack *= ADVANTAGE; }
	//void Disadvantage(void) { m_nAttack /= DISADVANTAGE; }
	void Disadvantage(void) { m_nAttack = 0; }
	void NoDamage(void) { m_nAttack = 0; }

private:
	
	void CanToEnemy(void);
	void CapsuleToEnemy(void);
	void IceToEnemy(void);
	void PetBottleToEnemy(void);
	void DustToEnemy(void);
	void SnackToEnemy(void);
	void CigaretToEnemy(void);
	void CardToEnemy(void);
	void BottleToEnemy(void);

	// メンバ変数
	int m_nAttack;			// 攻撃力
	CEnemy::TYPE m_Enemy;	// 敵の種類
};

#endif