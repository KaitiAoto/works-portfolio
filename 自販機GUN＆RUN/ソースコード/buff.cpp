//==================================
//
// バフ[buff.cpp]
// Author : Kaiti Aoto
//
//==================================

// インクルード
#include "buff.h"
#include "game.h"
#include "buff_icon.h"

//==================
// コンストラクタ
//==================
CBuff::CBuff()
{
    // 値をクリア
    m_type = TYPE_NONE;
}
//================
// デストラクタ
//================
CBuff::~CBuff()
{
}
//============
// 更新処理
//============
void CBuff::Update(float deltaTime)
{
    // バフがかかっていなければ
    if (m_bActive != true)
    {
        return; // 何もせず終了
    }

    // 効果時間を減らす
    m_fDuration -= deltaTime;
    
    // 時間が0以下なら
    if (m_fDuration <= 0.0f)
    {
        // スピードバフなら
        if (m_type == TYPE_SPEED)
        {
            
            CGame::GetPlayer()->AddSpeed(-m_fSpeed);    // 元の速度に戻す
            m_bActive = false;                          // バフがかかっていない状態にする
            m_fSpeed = 0.0f;                            // スピードを0に
            m_type = TYPE_NONE;                         // 種類を初期化
        }
    }
}
//======================
// スピードアップ処理
//======================
void CBuff::AddSpeed(float fSpeed, float fDuration)
{
    // すでにスピードバフがかかっていたら
    if (m_bActive && m_type == TYPE_SPEED)
    {
        m_fDuration = fDuration;    // 時間を最大にする
        return;                     // 何もせず終了
    }

    // 値を代入
    m_fSpeed = fSpeed;
    m_fDuration = fDuration;
    m_type = TYPE_SPEED;    // 種類をスピードにする
    m_bActive = true;       // バフがかかっている状態にする

    // プレイヤーに速度上昇を反映
    CGame::GetPlayer()->AddSpeed(m_fSpeed);

    // アイコンを生成
    CBuffIcon::Create({ BUFF_X / 2, SCREEN_HEIGHT / 2 + 100.0f,0.0f }, BUFF_X, BUFF_Y);

    // 画面フラッシュの色を変える
    CGame::GetFlash()->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f));
}