#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using KeyboardButton = unsigned long long;

// ゲームパッド
class Keyboard
{
public:
	enum State
	{
		Up,
		Down,
		UpMoment,
		DownMoment,
	};

	static const KeyboardButton W = (1 << 0);
	static const KeyboardButton A = (1 << 1);
	static const KeyboardButton S = (1 << 2);
	static const KeyboardButton D = (1 << 3);
	static const KeyboardButton Q = (1 << 4);
	static const KeyboardButton E = (1 << 5);
	static const KeyboardButton R = (1 << 6);
	static const KeyboardButton Z = (1 << 7);
	static const KeyboardButton X = (1 << 8);
	static const KeyboardButton C = (1 << 9);
	static const KeyboardButton F = (1 << 10);
	static const KeyboardButton V = (1 << 11);
	static const KeyboardButton I = (1 << 12);
	static const KeyboardButton J = (1 << 13);
	static const KeyboardButton K = (1 << 14);
	static const KeyboardButton L = (1 << 15);
	static const KeyboardButton U = (1 << 16);
	static const KeyboardButton O = (1 << 17);
	static const KeyboardButton P = (1 << 18);
	static const KeyboardButton SPACE = (1 << 19);
	static const KeyboardButton SHIFT = (1 << 20);
	static const KeyboardButton TAB = (1 << 21);
	static const KeyboardButton ESC = (1 << 22);
	static const KeyboardButton ENTER = (1 << 23);
	static const KeyboardButton UP = (1 << 24);
	static const KeyboardButton DOWN = (1 << 25);
	static const KeyboardButton LEFT = (1 << 26);
	static const KeyboardButton RIGHT = (1 << 27);
	static const KeyboardButton Key1 = (1 << 28);
	static const KeyboardButton Key2 = (1 << 29);
	static const KeyboardButton Key3 = (1 << 30);
	static const KeyboardButton Key4 = (1ULL << 31);
	static const KeyboardButton Key5 = (1ULL << 32);
	static const KeyboardButton Key6 = (1ULL << 33);
	static const KeyboardButton Key7 = (1ULL << 34);
	static const KeyboardButton Key8 = (1ULL << 35);
	static const KeyboardButton Key9 = (1ULL << 36);
	static const KeyboardButton Key0 = (1ULL << 37);
	static const KeyboardButton N = (1ULL << 38);
	static const KeyboardButton CTRL = (1ULL << 39);
	static const KeyboardButton F1 = (1ULL << 40);
	static const KeyboardButton F2 = (1ULL << 41);
	static const KeyboardButton F3 = (1ULL << 42);
	static const KeyboardButton F4 = (1ULL << 43);
	static const KeyboardButton F5 = (1ULL << 44);
	static const KeyboardButton F6 = (1ULL << 45);
	static const KeyboardButton F7 = (1ULL << 46);

public:
	Keyboard() {}
	~Keyboard() {}

	// 更新
	void Update();

	//キーの入力状態の取得
	bool GetKeyInput(KeyboardButton button,int state);

private:
	//[0]が今回の入力、[1]が前回の入力
	KeyboardButton input_key[2] = { 0 };
};
