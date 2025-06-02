#include "Mouse.h"

static const int KeyMap[] =
{
	VK_LBUTTON,		// ���{�^��
	VK_MBUTTON,		// ���{�^��
	VK_RBUTTON,		// �E�{�^��
};

// �R���X�g���N�^
Mouse::Mouse(HWND hWnd)
	: hWnd(hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	screen_width = static_cast<float>(rc.right - rc.left);
	screen_height = static_cast<float>(rc.bottom - rc.top);
}

// �X�V
void Mouse::Update()
{
	// �X�C�b�`���
	MouseButton newButtonState = 0;

	for (int i = 0; i < ARRAYSIZE(KeyMap); ++i)
	{
		if (::GetAsyncKeyState(KeyMap[i]) & 0x8000)
		{
			newButtonState |= (1 << i);
		}
	}

	// �z�C�[��
	wheel[1] = wheel[0];
	wheel[0] = 0;

	// �{�^�����X�V
	button_state[1] = button_state[0];	// �X�C�b�`����
	button_state[0] = newButtonState;

	button_down = ~button_state[1] & newButtonState;	// �������u��
	button_up = ~newButtonState & button_state[1];	// �������u��

	// �J�[�\���ʒu�̎擾
	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(hWnd, &cursor);

	// ��ʂ̃T�C�Y���擾����B
	RECT rc;
	GetClientRect(hWnd, &rc);
	screen_width = static_cast<float>(rc.right - rc.left);
	screen_height = static_cast<float>(rc.bottom - rc.top);

	// ��ʕ␳
	position_x[1] = position_x[0];
	position_y[1] = position_y[0];
	position_x[0] = static_cast<float>(cursor.x / static_cast<float>(screen_width) * static_cast<float>(screen_width));
	position_y[0] = static_cast<float>(cursor.y / static_cast<float>(screen_height) * static_cast<float>(screen_height));
}
