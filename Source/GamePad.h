#pragma once
#include <DirectXMath.h>
#include <Xinput.h>

using GamePadButton = unsigned int;

// �Q�[���p�b�h
class GamePad
{
public:
	enum State
	{
		Up,
		Down,
		UpMoment,
		DownMoment,
	};

	static const GamePadButton BTN_UP = (1 << 0);
	static const GamePadButton BTN_RIGHT = (1 << 1);
	static const GamePadButton BTN_DOWN = (1 << 2);
	static const GamePadButton BTN_LEFT = (1 << 3);
	static const GamePadButton BTN_A = (1 << 4);
	static const GamePadButton BTN_B = (1 << 5);
	static const GamePadButton BTN_X = (1 << 6);
	static const GamePadButton BTN_Y = (1 << 7);
	static const GamePadButton BTN_START = (1 << 8);
	static const GamePadButton BTN_BACK = (1 << 9);
	static const GamePadButton BTN_LEFT_THUMB = (1 << 10);
	static const GamePadButton BTN_RIGHT_THUMB = (1 << 11);
	static const GamePadButton BTN_LEFT_SHOULDER = (1 << 12);
	static const GamePadButton BTN_RIGHT_SHOULDER = (1 << 13);
	static const GamePadButton BTN_LEFT_TRIGGER = (1 << 14);
	static const GamePadButton BTN_RIGHT_TRIGGER = (1 << 15);

public:
	GamePad(int slot = 0);
	~GamePad() {}

	// �X�V
	void Update();
	void ConectCheck();

	// �X���b�g�ݒ�
	void SetSlot(int slot) { this->slot = slot; }

	bool GetGamePadInput(GamePadButton button, int state);

	//// �{�^�����͏�Ԃ̎擾
	//GamePadButton GetButton() const { return button_state[0]; }

	//// �{�^��������Ԃ̎擾
	//GamePadButton GetButtonDown() const { return buttonDown; }

	//// �{�^�������Ԃ̎擾
	//GamePadButton GetButtonUp() const { return buttonUp; }

	// ���X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisLX() const { return axisLx; }
	// ���X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisLY() const { return axisLy; }
	// ���X�e�B�b�N���͏�Ԃ̎擾
	DirectX::XMFLOAT2 GetAxisL() const { return { axisLx ,axisLy }; }

	// �E�X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisRX() const { return axisRx; }
	// �E�X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisRY() const { return axisRy; }
	// �E�X�e�B�b�N���͏�Ԃ̎擾
	DirectX::XMFLOAT2 GetAxisR() const { return { axisRx ,axisRy }; }

	// ���g���K�[���͏�Ԃ̎擾
	float GetTriggerL() const { return triggerL; }

	// �E�g���K�[���͏�Ԃ̎擾
	float GetTriggerR() const { return triggerR; }

	DirectX::XMFLOAT2 NormalizeLeftStick() { return normalize_left_stick; }
	float LeftStickLength() { return left_stick_length; }

	DirectX::XMFLOAT2 NormalizeRightStick() { return normalize_right_stick; }
	float RightStickLength() { return right_stick_length; }

	bool GetConect() { return conect; }
	float GetAdmitRange() { return admit_range; }
private:
	GamePadButton		button_state[2] = { 0 };
	GamePadButton		buttonDown = 0;
	GamePadButton		buttonUp = 0;
	float				axisLx = 0.0f;
	float				axisLy = 0.0f;
	float				axisRx = 0.0f;
	float				axisRy = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int					slot = 0;
	bool conect = false;
	float admit_range = 0.334f; //�X�e�B�b�N���͒l�̐�Βl�����̒l�ȏ�Ȃ���͂��ꂽ�Ƃ݂Ȃ�

public:
	float left_stick_length = 0.0f;
	DirectX::XMFLOAT2 normalize_left_stick = { 0,0 };

	float right_stick_length = 0.0f;
	DirectX::XMFLOAT2  normalize_right_stick = { 0,0 };
};
