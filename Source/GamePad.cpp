#include <windows.h>
#include <math.h>

#include "GamePad.h"

GamePad::GamePad(int slot)
{
	SetSlot(slot);
}

// 更新
void GamePad::Update()
{
	axisLx = axisLy = 0.0f;
	axisRx = axisRy = 0.0f;
	triggerL = triggerR = 0.0f;

	GamePadButton newButtonState = 0;

	// ボタン情報取得
	XINPUT_STATE xinputState;
	int _pad_state;
	_pad_state = XInputGetState(slot, &xinputState);
	if (_pad_state == ERROR_SUCCESS)
	{
		conect = true;
		//XINPUT_CAPABILITIES caps;
		//XInputGetCapabilities(m_slot, XINPUT_FLAG_GAMEPAD, &caps);
		XINPUT_GAMEPAD& pad = xinputState.Gamepad;

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)					newButtonState |= BTN_UP;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)				newButtonState |= BTN_RIGHT;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)				newButtonState |= BTN_DOWN;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)				newButtonState |= BTN_LEFT;
		if (pad.wButtons & XINPUT_GAMEPAD_A)						newButtonState |= BTN_A;
		if (pad.wButtons & XINPUT_GAMEPAD_B)						newButtonState |= BTN_B;
		if (pad.wButtons & XINPUT_GAMEPAD_X)						newButtonState |= BTN_X;
		if (pad.wButtons & XINPUT_GAMEPAD_Y)						newButtonState |= BTN_Y;
		if (pad.wButtons & XINPUT_GAMEPAD_START)					newButtonState |= BTN_START;
		if (pad.wButtons & XINPUT_GAMEPAD_BACK)						newButtonState |= BTN_BACK;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)				newButtonState |= BTN_LEFT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)				newButtonState |= BTN_RIGHT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)			newButtonState |= BTN_LEFT_SHOULDER;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)			newButtonState |= BTN_RIGHT_SHOULDER;
		if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_LEFT_TRIGGER;
		if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	newButtonState |= BTN_RIGHT_TRIGGER;

		if ((pad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pad.sThumbLX = 0;
			pad.sThumbLY = 0;
		}

		if ((pad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pad.sThumbRX = 0;
			pad.sThumbRY = 0;
		}

		triggerL = static_cast<float>(pad.bLeftTrigger) / 255.0f;
		triggerR = static_cast<float>(pad.bRightTrigger) / 255.0f;
		axisLx = static_cast<float>(pad.sThumbLX) / static_cast<float>(0x8000);
		axisLy = static_cast<float>(pad.sThumbLY) / static_cast<float>(0x8000);
		axisRx = static_cast<float>(pad.sThumbRX) / static_cast<float>(0x8000);
		axisRy = static_cast<float>(pad.sThumbRY) / static_cast<float>(0x8000);

		//if()

	}
	else if (_pad_state == ERROR_DEVICE_NOT_CONNECTED)
	{
		conect = false;
	}

	//今はOKやけど変更した方がいいかも
	// キーボードでエミュレーション
	float lx = 0.0f;
	float ly = 0.0f;
	float rx = 0.0f;
	float ry = 0.0f;
	if (slot == 0)
	{
		if ((GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP)) & 0x8000) ly = 1.0f;
		if ((GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT)) & 0x8000) lx = -1.0f;
		if ((GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN)) & 0x8000) ly = -1.0f;
		if ((GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT)) & 0x8000) lx = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000) newButtonState |= BTN_A;
		if (GetAsyncKeyState('K') & 0x8000) newButtonState |= BTN_B;
		if (GetAsyncKeyState('U') & 0x8000) newButtonState |= BTN_X;
		if (GetAsyncKeyState('I') & 0x8000) newButtonState |= BTN_Y;
		if (GetAsyncKeyState('O') & 0x8000) newButtonState |= BTN_RIGHT_SHOULDER;
		if (GetAsyncKeyState('L') & 0x8000) newButtonState |= BTN_RIGHT_TRIGGER;
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) newButtonState |= BTN_LEFT_SHOULDER;
		if (GetAsyncKeyState('P') & 0x8000) newButtonState |= BTN_START;
		if (GetAsyncKeyState('Q') & 0x8000) newButtonState |= BTN_BACK;
		if (GetAsyncKeyState(VK_UP) & 0x8000)	newButtonState |= BTN_UP;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	newButtonState |= BTN_RIGHT;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	newButtonState |= BTN_DOWN;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)	newButtonState |= BTN_LEFT;
	}

#if 1
	if (newButtonState & BTN_UP)    ly = 1.0f;
	if (newButtonState & BTN_RIGHT) lx = 1.0f;
	if (newButtonState & BTN_DOWN)  ly = -1.0f;
	if (newButtonState & BTN_LEFT)  lx = -1.0f;
#endif

	if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0)
	{
		float power = ::sqrtf(lx * lx + ly * ly);
		axisLx = lx / power;
		axisLy = ly / power;
	}

	if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0)
	{
		float power = ::sqrtf(rx * rx + ry * ry);
		axisRx = rx / power;
		axisRy = ry / power;
	}



	// ボタン情報の更新
	{
		button_state[1] = button_state[0];	// スイッチ履歴
		button_state[0] = newButtonState;

		buttonDown = ~button_state[1] & newButtonState;	// 押した瞬間
		buttonUp = ~newButtonState & button_state[1];	// 離した瞬間
	}

	////////////////////////////////////////////////////////////////
	left_stick_length = sqrtf(axisLx * axisLx + axisLy * axisLy);
	if (left_stick_length)
		normalize_left_stick = { axisLx / left_stick_length, axisLy / left_stick_length };

	right_stick_length = sqrtf(axisRx * axisRx + axisRy * axisRy);
	if (right_stick_length)
		normalize_right_stick = { axisRx / right_stick_length, axisRy / right_stick_length };
}

void GamePad::ConectCheck()
{
	XINPUT_STATE xinputState;
	int _pad_state;
	_pad_state = XInputGetState(slot, &xinputState);
	if (_pad_state == ERROR_SUCCESS)
	{
		conect = true;
	}
	else if (_pad_state == ERROR_DEVICE_NOT_CONNECTED)
	{
		conect = false;
	}
}

bool GamePad::GetGamePadInput(GamePadButton button, int state)
{
	switch (state)
	{
	case Up:
		if (~button_state[0] & ~button_state[1] & button)
		{
			return true;
		}
		break;
	case Down:
		if (button_state[0] & button_state[1] & button)
		{
			return true;
		}
		break;
	case UpMoment:
		if (~button_state[0] & button_state[1] & button)
		{
			return true;
		}
		break;
	case DownMoment:
		if (button_state[0] & ~button_state[1] & button)
		{
			return true;
		}
		break;
	}

	return false;
}
