//===============================================
//
//  入力処理[input.h]
//  Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _INPUT_H_
#define _INPUT_H_

// インクルード
#include "main.h"

// 入力クラス
class CInput
{
public:

	// メンバ関数
	CInput();
	virtual ~CInput() = 0;
	virtual HRESULT Init(HINSTANCE hInstance);
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

protected:
	static constexpr int  NUM_KEY_MAX = 256;	// 最大キー数

	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInput; // 入力
};

// キー入力クラス
class CInputKey :public CInput
{
public:

	// メンバ関数
	CInputKey();
	~CInputKey();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	// 取得
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:
	// メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];	// キーの状態
	BYTE m_aOldState[NUM_KEY_MAX];	// 前の状態
	LPDIRECTINPUTDEVICE8 m_pDevice;	// デバイス
};

// パッドクラス
class CInputPad :public CInput
{
public:
	// キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,
		JOYKEY_DOWN,
		JOYKEY_LEFT,
		JOYKEY_RIGET,
		JOYKEY_START,
		JOYKEY_BACK,
		JOYKEY_L3,
		JOYKEY_R3,
		JOYKEY_L1,
		JOYKEY_R1,
		JOYKEY_L2,
		JOYKEY_R2,
		JOYKEY_A,
		JOYKEY_B,
		JOYKEY_X,
		JOYKEY_Y,
		JOYKEY_MAX
	}JOYKEY;

	// メンバ関数
	CInputPad();
	~CInputPad();
	HRESULT Init(HINSTANCE hInstance);
	void Uninit(void);
	void Update(void);
	void SetVibration(WORD leftMotor, WORD rightMotor, DWORD dwUserIndex, int nFrame);

	// 取得
	bool GetPress(JOYKEY nKey);
	bool GetTrigger(JOYKEY nKey);
	bool GetJoyStick();
	bool GetStickTriggerUp();
	bool GetStickTriggerDown();
	bool GetR2Trigger(BYTE threshold);
	bool GetL2Trigger(BYTE threshold);
	bool GetR2Press(BYTE threshold);
	bool GetL2Press(BYTE threshold);
	XINPUT_STATE* GetJoyStickAngle(void);
	float GetRightStickX();
	float GetRightStickY();
private:
	// メンバ変数
	XINPUT_STATE m_joyKeyState;			// ジョイパッドの状態
	XINPUT_STATE m_joyKeyStateTrigger;	// ジョイパッドのトリガー
	SHORT m_prevLY;						// 左スティックのＹ
	BYTE m_prevR2Trigger;				// R2のトリガー
	BYTE m_prevL2Trigger;				// L2のトリガー
	int m_nVibFrame;

};

// マウスクラス
class CInputMouse :public CInput
{
public:

	// メンバ関数
	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void ResetWheel(void) { m_wheel = 0; };

	// 取得
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
	int GetRelX(void);
	int GetRelY(void);
	int GetX(void);
	int GetY(void);
	int GetWheel(void);

private:

	// メンバ変数
	BYTE m_aButtonState[8];				// 現在のボタン状態
	BYTE m_aOldButtonState[8];			// 前フレームのボタン状態
	int m_relX;							// 移動量（X軸）
	int m_relY;							// 移動量（Y軸）
	int m_mouseX;						// カーソルの絶対位置（X座標）
	int m_mouseY;						// カーソルの絶対位置（Y座標）
	int m_wheel;						// ホイールの1フレーム変化量
	LPDIRECTINPUTDEVICE8 m_pDevice;		// デバイス
};

#endif
