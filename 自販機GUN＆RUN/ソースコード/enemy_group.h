//==============================
//
// 敵グループ[enemy_group.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _ENEMY_GROUP_H_
#define _ENEMY_GROUP_H_

// インクルード
#include "main.h"
#include "object.h"

// マクロ定義
#define ENEMYGROUP_RESPAWN (2400)

// 敵グループクラス
class CEnemyGroup:public CObject
{
public:
	// メンバ関数
	CEnemyGroup(int nPriority = PRIORITY_ENEMYBASE);
	~CEnemyGroup();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SubMyEnemy(void) { m_MyEnemy--; }

	// 設定
	void SetRespawn(bool bUse) { m_bRespawn = bUse; }

	// 取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	bool GetUse(void) { return m_bUse; }

	// 静的メンバ関数
	static CEnemyGroup* Create(const D3DXVECTOR3 pos);

private:
	void CreateEnemy(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	bool m_bUse;			// 使用しているか
	bool m_bRespawn;		// 復活できるか
	int m_nRespawn;			// 復活までの時間カウント
	int m_MyEnemy;			// グループに含まれる敵数
};

#endif