//===========================
//
//  入力処理[input.cpp]
//  Author : Kaiti Aoto
//
//===========================
#include "input.h"
//静的メンバ変数
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//==================
// コンストラクタ
//==================
CInput::CInput()
{
}
//================
// デストラクタ
//================
CInput::~CInput()
{
}
//=======================
// 初期化処理
//=======================
HRESULT CInput::Init(HINSTANCE hInstance)
{
	if (m_pInput == NULL)
	{
		//初期化
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//=======================
// 終了処理
//=======================
void CInput::Uninit(void)
{
	////入力デバイスの破棄
	//if (m_pDevice != NULL)
	//{
	//	m_pDevice->Unacquire();
	//	m_pDevice->Release();
	//	m_pDevice = NULL;
	//}
	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}


//==========================================
// 
// キー入力
// 
//==========================================
//==================
// コンストラクタ
//==================
CInputKey::CInputKey()
{
	memset(m_aKeyState, 0, sizeof(m_aKeyState));
	memset(m_aOldState, 0, sizeof(m_aOldState));
}
//================
// デストラクタ
//================
CInputKey::~CInputKey()
{
}
//=======================
//キーボードの初期化処理
//=======================
HRESULT CInputKey::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance);

	//
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}
//=======================
//キーボードの終了処理
//=======================
void CInputKey::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	CInput::Uninit();
}
//=======================
//キーボードの更新処理
//=======================
void CInputKey::Update(void)
{
	if (!m_pDevice)
		return; // NULL チェック

	BYTE aKeyState[NUM_KEY_MAX] = {};
	memcpy(m_aOldState, m_aKeyState, sizeof(m_aKeyState));

	HRESULT hr = m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0]);
	if (FAILED(hr))
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0]); // 再取得

#ifdef _DEBUG
		OutputDebugStringA("GetDeviceState failed. Trying to Acquire...\n");
#endif // _DEBUG

	}
	if (SUCCEEDED(hr))
	{
		memcpy(m_aKeyState, aKeyState, sizeof(m_aKeyState));
	}
	else
	{
		memset(m_aKeyState, 0, sizeof(m_aKeyState));  // 失敗時はクリア
	}
}
//==============================
//キーボードのプレス情報を取得
//==============================
bool CInputKey::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x080) ? true : false;
}
//================================
//キーボードのトリガー情報を取得
//================================
bool CInputKey::GetTrigger(int nKey)
{
	bool Trigger = false;
	if (m_aKeyState[nKey] & 0x80 && !(m_aOldState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool CInputKey::GetRelease(int nKey)
{
	bool Trigger = false;
	if (m_aOldState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool CInputKey::GetRepeat(int nKey)
{
	bool Trigger = false;
	if (m_aOldState[nKey] & 0x80 && (m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}


//===================================
// 
// コントローラー
// 
//===================================
//==================
// コンストラクタ
//==================
CInputPad::CInputPad()
{
	m_joyKeyState = {};
	m_joyKeyStateTrigger = {};
}
//================
// デストラクタ
//================
CInputPad::~CInputPad()
{
}
//============================
//コントローラーの初期化処理
//============================
HRESULT CInputPad::Init(HINSTANCE hInstance)
{
	CInput::Init(hInstance);
	//メモリのクリア
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効)
	XInputEnable(true);

	return S_OK;
}
//============================
//コントローラーの終了処理
//============================
void CInputPad::Uninit(void)
{
	//XInputのステートを設定(無効)
	XInputEnable(false);

	CInput::Uninit();
}
//============================
//コントローラーの更新処理
//============================
void CInputPad::Update(void)
{
	XINPUT_STATE joykeyState;
	//
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = m_joyKeyState.Gamepad.wButtons;

		m_joyKeyStateTrigger.Gamepad.wButtons = Button & ~OldButton;

		m_prevLY = m_joyKeyState.Gamepad.sThumbLY;

		// R/L2トリガーの前回値を保存
		m_prevR2Trigger = m_joyKeyState.Gamepad.bRightTrigger;
		m_prevL2Trigger = m_joyKeyState.Gamepad.bLeftTrigger;

		m_joyKeyState = joykeyState;
	}

	m_nVibFrame--;
	if (m_nVibFrame <= 0)
	{
		XINPUT_VIBRATION vibration{};
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		XInputSetState(0, &vibration);
	}
}
//=================================
//コントローラーのプレス情報を取得
//=================================
bool CInputPad::GetPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
//===================================
//ジョイパッドのトリガー情報を取得
//===================================
bool CInputPad::GetTrigger(JOYKEY key)
{
	return(m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
bool CInputPad::GetJoyStick()
{
	bool joykey = false;

	if (m_joyKeyState.Gamepad.sThumbLX >= 100
		|| m_joyKeyState.Gamepad.sThumbLX <= -100
		|| m_joyKeyState.Gamepad.sThumbLY >= 100
		|| m_joyKeyState.Gamepad.sThumbLY <= -100)
	{
		joykey = true;
	}
	return joykey;
}
XINPUT_STATE* CInputPad::GetJoyStickAngle(void)
{
	return &m_joyKeyState;
}
float CInputPad::GetRightStickX()
{
	return (m_joyKeyState.Gamepad.sThumbRX / 32768.0f);
}
float CInputPad::GetRightStickY()
{
	return (m_joyKeyState.Gamepad.sThumbRY / 32768.0f);
}
bool CInputPad::GetStickTriggerUp()
{
	const int DEADZONE = 10000;
	SHORT cur = m_joyKeyState.Gamepad.sThumbLY;
	return (cur > DEADZONE && m_prevLY <= DEADZONE);
}
bool CInputPad::GetStickTriggerDown()
{
	const int DEADZONE = 10000;
	SHORT cur = m_joyKeyState.Gamepad.sThumbLY;
	return (cur < -DEADZONE && m_prevLY >= -DEADZONE);
}

bool CInputPad::GetR2Trigger(BYTE threshold)
{
	return (m_joyKeyState.Gamepad.bRightTrigger > threshold && m_prevR2Trigger <= threshold);
}

bool CInputPad::GetL2Trigger(BYTE threshold)
{
	return (m_joyKeyState.Gamepad.bLeftTrigger > threshold && m_prevL2Trigger <= threshold);
}

bool CInputPad::GetR2Press(BYTE threshold)
{
	return (m_joyKeyState.Gamepad.bRightTrigger > threshold);
}

bool CInputPad::GetL2Press(BYTE threshold)
{
	return (m_joyKeyState.Gamepad.bLeftTrigger > threshold);
}
//============================
// パッド振動の設定
//============================
void CInputPad::SetVibration(WORD leftMotor, WORD rightMotor, DWORD dwUserIndex, int nFrame)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	// 左右モーターの強さを設定
	vibration.wLeftMotorSpeed = leftMotor;   // 左モーター（低周波・強い振動）
	vibration.wRightMotorSpeed = rightMotor;  // 右モーター（高周波・細かい振動）

	// 実際に振動をセット
	XInputSetState(dwUserIndex, &vibration);

	m_nVibFrame = nFrame;
}

//==========================================
// 
// マウス
// 
//==========================================
//==================
// コンストラクタ
//==================
CInputMouse::CInputMouse()
{
	memset(m_aButtonState, 0, sizeof(m_aButtonState));
	memset(m_aOldButtonState, 0, sizeof(m_aOldButtonState));
	m_mouseX = 0;
	m_mouseY = 0;
	m_relX = 0;
	m_relY = 0;
}
//================
// デストラクタ
//================
CInputMouse::~CInputMouse()
{
}
//=======================
// マウスの初期化処理
//=======================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance);

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2))) // マウス2ボタン以上対応フォーマット
	{
		return E_FAIL;
	}
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}
	m_pDevice->Acquire();

	return S_OK;
}
//=======================
// マウスの終了処理
//=======================
void CInputMouse::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	CInput::Uninit();
}
//=======================
// マウスの更新処理
//=======================
void CInputMouse::Update(void)
{
	if (!m_pDevice)
		return;

	DIMOUSESTATE2 mouseState = {};
	memcpy(m_aOldButtonState, m_aButtonState, sizeof(m_aButtonState));

	HRESULT hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	if (FAILED(hr))
	{
		m_pDevice->Acquire();
		hr = m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);

#ifdef _DEBUG
		OutputDebugStringA("Mouse GetDeviceState failed. Trying to Acquire...\n");
#endif
	}
	if (SUCCEEDED(hr))
	{
		for (int i = 0; i < 8; ++i) // ボタン数は最大8個
		{
			m_aButtonState[i] = (mouseState.rgbButtons[i] & 0x80) ? true : false;
		}
		// 相対移動量
		m_relX = mouseState.lX;
		m_relY = mouseState.lY;

		// マウス座標の累積更新
		m_mouseX += m_relX;
		m_mouseY += m_relY;

		//ホイールの変化量を記録
		m_wheel = mouseState.lZ;
	}
	else
	{
		memset(m_aButtonState, 0, sizeof(m_aButtonState));
		m_relX = 0;
		m_relY = 0;
	}
}
//=======================
// マウスのボタン押下判定
//=======================
bool CInputMouse::GetPress(int button)
{
	if (button < 0 || button >= 8) return false;
	return m_aButtonState[button];
}
//=======================
// マウスのボタントリガー判定
//=======================
bool CInputMouse::GetTrigger(int button)
{
	if (button < 0 || button >= 8) return false;
	return (m_aButtonState[button] && !m_aOldButtonState[button]);
}
//=======================
// マウスのボタンリリース判定
//=======================
bool CInputMouse::GetRelease(int button)
{
	if (button < 0 || button >= 8) return false;
	return (!m_aButtonState[button] && m_aOldButtonState[button]);
}
//=======================
// マウスの相対移動量取得
//=======================
int CInputMouse::GetRelX(void)
{
	return m_relX;
}
int CInputMouse::GetRelY(void)
{
	return m_relY;
}
//=======================
// マウスの絶対座標取得（累積値）
//=======================
int CInputMouse::GetX(void)
{
	return m_mouseX;
}
int CInputMouse::GetY(void)
{
	return m_mouseY;
}
//
//
//
int CInputMouse::GetWheel(void)
{
	return m_wheel;
}