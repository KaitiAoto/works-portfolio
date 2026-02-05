//===============================================
//
// ボスの攻撃[bossAttack.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _BOSSATTACK_H_
#define _BOSSATTACK_H_

// 前方宣言
class CDamegeArea;

// ボスの攻撃クラス
class CBossAttack
{
public:
	// 種類
	typedef enum
	{
		TYPE_NONE = -1,	// 何もしない
		TYPE_THROW = 0,	// 投げつけ
		TYPE_SWEEP,		// 薙ぎ払い
		TYPE_FIRE,		// 火炎放射
		TYPE_THUNDER,	// 落雷
		TYPE_SLAM,		// 叩き下ろし
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CBossAttack();
	~CBossAttack();
	void Throw(void);
	void Sweep(void);
	void Fire(void);
	void Thunder(void);
	void Slam(void);
	void AttackCancel(int nDamegeTime);

	// 設定処理
	void SetAttack(D3DXVECTOR3 pos) { m_TargetPos = pos;}
	void SetStay(bool bStay) { m_bStay = bStay; }
	void SetType(TYPE type) { m_type = type; }
	// 取得処理
	TYPE GetType(void) { return m_type; }
	CDamegeArea* GetArea(void) { return m_pArea; }

private:

	// メンバ変数
	D3DXVECTOR3 m_TargetPos;	// ターゲット位置
	D3DXVECTOR3 m_AreaPos;		// エリア位置
	CDamegeArea* m_pArea;		// 攻撃エリア
	int m_nCntDamegeTime;		// ダメージ時間
	bool m_bStay;				// 待機するかどうか
	TYPE m_type;				// 種類
};

#endif