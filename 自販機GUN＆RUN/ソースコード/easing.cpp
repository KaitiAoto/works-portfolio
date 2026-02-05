//==============================
//
//  イージング処理[easing.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "easing.h"

//==================
// デストラクタ
//==================
CEasing::~CEasing()
{
}
//==================
// アウトバウンス
//==================
float CEasing::OutBounce(float t)
{
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (t < 1.0f / d1) {
        return n1 * t * t;
    }
    else if (t < 2.0f / d1) {
        return n1 * (t -= 1.5f / d1) * t + 0.75f;
    }
    else if (t < 2.5f / d1) {
        return n1 * (t -= 2.25f / d1) * t + 0.9375f;
    }
    else {
        return n1 * (t -= 2.625f / d1) * t + 0.984375f;
    }
}

float CEasing::OutElastic(float fX)
{
    const float c4 = (2.0f * D3DX_PI) / 3.0f;

    if (fX == 0.0f)
        return 0.0f;
    else if (fX == 1.0f)
        return 1.0f;
    else
        return powf(2.0f, -10.0f * fX) * sinf((fX * 10.0f - 0.75f) * c4) + 1.0f;
}

float CEasing::OutExpo(float fX)
{
    if (fX == 1.0f)
    {
        return 1.0f;
    }
    else
    {
        return 1.0f - powf(2.0f, -10.0f * fX);
    }
}

float CEasing::OutBack(float fX)
{
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;

    return 1.0f + c3 * pow(fX - 1.0f, 3.0f) + c1 * pow(fX - 1.0f, 2.0f);
}
