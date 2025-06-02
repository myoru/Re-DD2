#include "Mouse.h"

static const int KeyMap[] =
{
	VK_LBUTTON,		// 左ボタン
	VK_MBUTTON,		// 中ボタン
	VK_RBUTTON,		// 右ボタン
};

// コンストラクタ
Mouse::Mouse(HWND hWnd)
	: hWnd(hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	screen_width = static_cast<float>(rc.right - rc.left);
	screen_height = static_cast<float>(rc.bottom - rc.top);
}

// 更新
void Mouse::Update()
{
	// スイッチ情報
	MouseButton newButtonState = 0;

	for (int i = 0; i < ARRAYSIZE(KeyMap); ++i)
	{
		if (::GetAsyncKeyState(KeyMap[i]) & 0x8000)
		{
			newButtonState |= (1 << i);
		}
	}

	// ホイール
	wheel[1] = wheel[0];
	wheel[0] = 0;

	// ボタン情報更新
	button_state[1] = button_state[0];	// スイッチ履歴
	button_state[0] = newButtonState;

	button_down = ~button_state[1] & newButtonState;	// 押した瞬間
	button_up = ~newButtonState & button_state[1];	// 離した瞬間

	// カーソル位置の取得
	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(hWnd, &cursor);

	// 画面のサイズを取得する。
	RECT rc;
	GetClientRect(hWnd, &rc);
	screen_width = static_cast<float>(rc.right - rc.left);
	screen_height = static_cast<float>(rc.bottom - rc.top);

	// 画面補正
	position_x[1] = position_x[0];
	position_y[1] = position_y[0];
	position_x[0] = static_cast<float>(cursor.x / static_cast<float>(screen_width) * static_cast<float>(screen_width));
	position_y[0] = static_cast<float>(cursor.y / static_cast<float>(screen_height) * static_cast<float>(screen_height));
}
