//==============================
//
// 相性[matchup.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "matchup.h"
#include "manager.h"
#include "camera.h"

//==================
// コンストラクタ
//==================
CMatchUp::CMatchUp()
{
	m_nAttack = 0;
	m_Enemy = CEnemy::TYPE_LIGHTER;
}
//================
// デストラクタ
//================
CMatchUp::~CMatchUp()
{

}
//===================
// 相性を調べる処理
//===================
int CMatchUp::Check(CBullet::TYPE Bullet, CEnemy::TYPE Enemy)
{	
	m_nAttack = BULLET_ATTACK;
	m_Enemy = Enemy;

	switch (Bullet)
	{
	case CBullet::TYPE_CAN:
		CanToEnemy();
		break;
	case CBullet::TYPE_CAPSULE:
		CapsuleToEnemy();
		break;
	case CBullet::TYPE_ICE:
		IceToEnemy();
		break;
	case CBullet::TYPE_PETBOTTLE:
		PetBottleToEnemy();
		break;
	case CBullet::TYPE_DUST:
		DustToEnemy();
		break;
	case CBullet::TYPE_SNACK:
		SnackToEnemy();
		break;
	case CBullet::TYPE_CIGARET:
		CigaretToEnemy();
		break;
	case CBullet::TYPE_CARD:
		CardToEnemy();
		break;
	case CBullet::TYPE_BOTTLE:
		BottleToEnemy();
		break;

	default:
		break;
	}

	return m_nAttack;
}
//====================
// 缶との相性を調べる
//====================
void CMatchUp::CanToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		break;
	case CEnemy::TYPE_MONEY0:
		break;
	case CEnemy::TYPE_MONEY1:
		break;
	case CEnemy::TYPE_MAGNET:
		NoDamage();
		break;
	case CEnemy::TYPE_TORNADO:
		break;
	case CEnemy::TYPE_SPRAY:
		break;
	case CEnemy::TYPE_EXT:
		break;
	case CEnemy::TYPE_KETTLE:
		break;
	case CEnemy::TYPE_RANGE:
		Advantage();
		break;

	default:
		break;
	}
}
//===========================
// カプセルとの相性を調べる
//===========================
void CMatchUp::CapsuleToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		Disadvantage();
		break;
	case CEnemy::TYPE_MONEY0:
		Advantage();
		break;
	case CEnemy::TYPE_MONEY1:
		Advantage();
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		Disadvantage();
		break;
	case CEnemy::TYPE_SPRAY:
		break;
	case CEnemy::TYPE_EXT:
		Disadvantage();
		break;
	case CEnemy::TYPE_KETTLE:
		break;
	case CEnemy::TYPE_RANGE:
		Disadvantage();
		break;
	default:
		break;
	}
}
//===========================
// アイスとの相性を調べる
//===========================
void CMatchUp::IceToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		NoDamage();
		break;
	case CEnemy::TYPE_MONEY0:	
		break;
	case CEnemy::TYPE_MONEY1:
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		Disadvantage();
		break;
	case CEnemy::TYPE_SPRAY:
		Disadvantage();
		break;
	case CEnemy::TYPE_EXT:
		break;
	case CEnemy::TYPE_KETTLE:
		Disadvantage();
		break;
	case CEnemy::TYPE_RANGE:
		NoDamage();
		break;

	default:
		break;
	}
}
//================================
// ペットボトルとの相性を調べる
//================================
void CMatchUp::PetBottleToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		Advantage();
		break;
	case CEnemy::TYPE_MONEY0:
		break;
	case CEnemy::TYPE_MONEY1:
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		break;
	case CEnemy::TYPE_SPRAY:
		Disadvantage();
		break;
	case CEnemy::TYPE_EXT:
		break;
	case CEnemy::TYPE_KETTLE:
		Disadvantage();
		break;
	case CEnemy::TYPE_RANGE:
		Disadvantage();
		break;

	default:
		break;
	}
}
//===========================
// 空き缶との相性を調べる
//===========================
void CMatchUp::DustToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		Disadvantage();
		break;
	case CEnemy::TYPE_MONEY0:
		Disadvantage();
		break;
	case CEnemy::TYPE_MONEY1:
		Disadvantage();
		break;
	case CEnemy::TYPE_MAGNET:
		NoDamage();
		break;
	case CEnemy::TYPE_TORNADO:
		Disadvantage();
		break;
	case CEnemy::TYPE_SPRAY:
		break;
	case CEnemy::TYPE_EXT:
		Disadvantage();
		break;
	case CEnemy::TYPE_KETTLE:
		Advantage();
		break;
	case CEnemy::TYPE_RANGE:
		Advantage();
		break;

	default:
		break;
	}
}
//===========================
// お菓子との相性を調べる
//===========================
void CMatchUp::SnackToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		break;
	case CEnemy::TYPE_MONEY0:
		break;
	case CEnemy::TYPE_MONEY1:
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		Disadvantage();
		break;
	case CEnemy::TYPE_SPRAY:
		Disadvantage();
		break;
	case CEnemy::TYPE_EXT:
		break;
	case CEnemy::TYPE_KETTLE:
		break;
	case CEnemy::TYPE_RANGE:
		break;

	default:
		break;
	}
}
//===========================
// タバコとの相性を調べる
//===========================
void CMatchUp::CigaretToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		NoDamage();
		break;
	case CEnemy::TYPE_MONEY0:
		Disadvantage();
		break;
	case CEnemy::TYPE_MONEY1:
		Disadvantage();
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		Disadvantage();
		break;
	case CEnemy::TYPE_SPRAY:
		break;
	case CEnemy::TYPE_EXT:
		NoDamage();
		break;
	case CEnemy::TYPE_KETTLE:
		Disadvantage();
		break;
	case CEnemy::TYPE_RANGE:
		break;

	default:
		break;
	}

}
//===========================
// カードとの相性を調べる
//===========================
void CMatchUp::CardToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		Disadvantage();
		break;
	case CEnemy::TYPE_MONEY0:
		Advantage();
		break;
	case CEnemy::TYPE_MONEY1:
		Advantage();
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		NoDamage();
		break;
	case CEnemy::TYPE_SPRAY:
		Disadvantage();
		break;
	case CEnemy::TYPE_EXT:
		NoDamage();
		break;
	case CEnemy::TYPE_KETTLE:
		NoDamage();
		break;
	case CEnemy::TYPE_RANGE:
		break;

	default:
		break;
	}

}
//===========================
// 瓶との相性を調べる
//===========================
void CMatchUp::BottleToEnemy(void)
{
	switch (m_Enemy)
	{
	case CEnemy::TYPE_LIGHTER:
		Advantage();
		break;
	case CEnemy::TYPE_MONEY0:
		break;
	case CEnemy::TYPE_MONEY1:
		break;
	case CEnemy::TYPE_MAGNET:
		break;
	case CEnemy::TYPE_TORNADO:
		Advantage();
		break;
	case CEnemy::TYPE_SPRAY:
		Advantage();
		break;
	case CEnemy::TYPE_EXT:
		break;
	case CEnemy::TYPE_KETTLE:
		break;
	case CEnemy::TYPE_RANGE:
		Advantage();
		break;

	default:
		break;
	}

}
