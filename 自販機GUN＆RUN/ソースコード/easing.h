//==============================
//
//  イージング処理[easing.h]
//  Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _EASING_H_
#define _EASING_H_

// イージングクラス
class CEasing
{
public:
	// メンバ関数
	~CEasing();
	
	// 静的メンバ関数
	static float OutBounce(float fX);
	static float OutElastic(float fX);
	static float OutExpo(float fX);
	static float OutBack(float fX);

private:
	CEasing() {};
};

#endif