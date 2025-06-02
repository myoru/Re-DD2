#pragma once

#include <Windows.h>

using MouseButton = unsigned int;

// マウス
class Mouse
{
public:
	static const MouseButton BTN_LEFT = (1 << 0);
	static const MouseButton BTN_MIDDLE = (1 << 1);
	static const MouseButton BTN_RIGHT = (1 << 2);

public:
	Mouse(HWND hWnd);
	~Mouse() {}

	// 更新
	void Update();

	// ボタン入力状態の取得
	MouseButton GetButton() const { return button_state[0]; }

	// ボタン押下状態の取得
	MouseButton GetButtonDown() const { return button_down; }

	// ボタン押上状態の取得
	MouseButton GetButtonUp() const { return button_up; }

	// ホイール値の設定
	void SetWheel(int wheel) { this->wheel[0] += wheel; }

	// ホイール値の取得
	int GetWheel() const { return wheel[1]; }

	// マウスカーソルX座標取得
	float GetPositionX() const { return position_x[0]; }

	// マウスカーソルY座標取得
	float GetPositionY() const { return position_y[0]; }

	// 前回のマウスカーソルX座標取得
	float GetOldPositionX() const { return position_x[1]; }

	// 前回のマウスカーソルY座標取得
	float GetOldPositionY() const { return position_y[1]; }

	// スクリーン幅設定
	void SetScreenWidth(int width) { screen_width = width; }

	// スクリーン高さ設定
	void SetScreenHeight(int height) { screen_height = height; }

	// スクリーン幅取得
	int GetScreenWidth() const { return screen_width; }

	// スクリーン高さ取得
	int GetScreenHeight() const { return screen_height; }

private:
	MouseButton		button_state[2] = { 0 };
	MouseButton		button_down = 0;
	MouseButton		button_up = 0;
	float			position_x[2];
	float			position_y[2];
	int				wheel[2];
	int				screen_width = 0;
	int				screen_height = 0;
	HWND			hWnd = nullptr;

public:
	int tete;
	int tete2;

};





//#pragma once
//#include <dinput.h>
//
//class Mouse
//{
//public:
//	Mouse() {};
//	~Mouse() {};
//
//
//	// DirectInputオブジェクト
//	LPDIRECTINPUT8 dinput;
//
//	// マウスデバイス
//	LPDIRECTINPUTDEVICE8 mouseDevice;
//
//
//	// DirectInputの初期化
//	void InitDirectInput(HWND hwnd) {
//		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, NULL);
//		dinput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
//		mouseDevice->SetDataFormat(&c_dfDIMouse2);
//		mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//		mouseDevice->Acquire();
//	}
//
//	// マウスの状態を取得
//	void UpdateMouseState() {
//		DIMOUSESTATE2 mouseState;
//		mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
//
//		// ホイールのスクロール量を取得
//		int wheelDelta = mouseState.lZ;
//	}
//
//
//
//public:
//	INPUT input = {};
//	POINT mouse_old_pos = {};//ボタンを押した瞬間のマウス座標
//	POINT mouse_new_pos = {};//今のマウス座標
//	POINT mouse_finish_pos[2] = {};//ボタンを離した時のマウス座標(0が左ボタン,1が右ボタン)
//	DirectX::XMVECTOR mouse_vec = {};
//	bool is_click[2] = { false,false };//trueならボタンを押している(0が左ボタン,1が右ボタン)
//	int old_wheel = 0;
//	int now_wheel = 0;
//	bool is_wheel = false;
//
//};