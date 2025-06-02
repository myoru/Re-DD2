#include "Input.h"

Input* Input::instance = nullptr;

// �R���X�g���N�^
Input::Input(HWND hWndint, int padNum)
	: mouse(hWndint)
{
	for (int i = 0; i < PAD_MAX; i++)
	{
		GamePad gamePad(i);
		gamePads.emplace_back(gamePad);
	}

	instance = this;

}

// �X�V����
void Input::Update(int padNum)
{
	mouse.Update();

	keyboard.Update();

	for (GamePad& _gamePad : gamePads)
	{
		_gamePad.ConectCheck();
	}
}
