#pragma once

#include <Windows.h>

using MouseButton = unsigned int;

// �}�E�X
class Mouse
{
public:
	static const MouseButton BTN_LEFT = (1 << 0);
	static const MouseButton BTN_MIDDLE = (1 << 1);
	static const MouseButton BTN_RIGHT = (1 << 2);

public:
	Mouse(HWND hWnd);
	~Mouse() {}

	// �X�V
	void Update();

	// �{�^�����͏�Ԃ̎擾
	MouseButton GetButton() const { return button_state[0]; }

	// �{�^��������Ԃ̎擾
	MouseButton GetButtonDown() const { return button_down; }

	// �{�^�������Ԃ̎擾
	MouseButton GetButtonUp() const { return button_up; }

	// �z�C�[���l�̐ݒ�
	void SetWheel(int wheel) { this->wheel[0] += wheel; }

	// �z�C�[���l�̎擾
	int GetWheel() const { return wheel[1]; }

	// �}�E�X�J�[�\��X���W�擾
	float GetPositionX() const { return position_x[0]; }

	// �}�E�X�J�[�\��Y���W�擾
	float GetPositionY() const { return position_y[0]; }

	// �O��̃}�E�X�J�[�\��X���W�擾
	float GetOldPositionX() const { return position_x[1]; }

	// �O��̃}�E�X�J�[�\��Y���W�擾
	float GetOldPositionY() const { return position_y[1]; }

	// �X�N���[�����ݒ�
	void SetScreenWidth(int width) { screen_width = width; }

	// �X�N���[�������ݒ�
	void SetScreenHeight(int height) { screen_height = height; }

	// �X�N���[�����擾
	int GetScreenWidth() const { return screen_width; }

	// �X�N���[�������擾
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
//	// DirectInput�I�u�W�F�N�g
//	LPDIRECTINPUT8 dinput;
//
//	// �}�E�X�f�o�C�X
//	LPDIRECTINPUTDEVICE8 mouseDevice;
//
//
//	// DirectInput�̏�����
//	void InitDirectInput(HWND hwnd) {
//		DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, NULL);
//		dinput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
//		mouseDevice->SetDataFormat(&c_dfDIMouse2);
//		mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//		mouseDevice->Acquire();
//	}
//
//	// �}�E�X�̏�Ԃ��擾
//	void UpdateMouseState() {
//		DIMOUSESTATE2 mouseState;
//		mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
//
//		// �z�C�[���̃X�N���[���ʂ��擾
//		int wheelDelta = mouseState.lZ;
//	}
//
//
//
//public:
//	INPUT input = {};
//	POINT mouse_old_pos = {};//�{�^�����������u�Ԃ̃}�E�X���W
//	POINT mouse_new_pos = {};//���̃}�E�X���W
//	POINT mouse_finish_pos[2] = {};//�{�^���𗣂������̃}�E�X���W(0�����{�^��,1���E�{�^��)
//	DirectX::XMVECTOR mouse_vec = {};
//	bool is_click[2] = { false,false };//true�Ȃ�{�^���������Ă���(0�����{�^��,1���E�{�^��)
//	int old_wheel = 0;
//	int now_wheel = 0;
//	bool is_wheel = false;
//
//};