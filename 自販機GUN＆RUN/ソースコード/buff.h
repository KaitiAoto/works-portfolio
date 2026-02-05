//==============================
//
// バフ[buff.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _BUFF_H_
#define _BUFF_H_

// バフクラス
class CBuff
{
public:
	// 種類
	enum TYPE
	{
		TYPE_NONE,
		TYPE_SPEED,
		TYPE_MAX
	};

	// メンバ関数
	CBuff();
	~CBuff();

	void Update(float deltaTime);
	void AddSpeed(float fSpeed, float fDuration);

	// 取得
	TYPE GetType(void) { return m_type; }
private:
	//メンバ変数
	float m_fSpeed;		// スピード
	float m_fDuration;	// 効果時間
	bool m_bActive;		// バフがかかっているか
	TYPE m_type;		// 種類
};

#endif