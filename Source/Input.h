#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "GamePad.h"
#include <vector>

#define PAD_MAX 5

// インプット
class Input
{
public:
	Input(HWND hWnd, int padNum = 0);
	~Input() {}

public:
	// インスタンス取得
	static Input& Instance() { return *instance; }

	// 更新処理
	void Update(int padNum = 0);

	// マウス取得
	Mouse& GetMouse() { return mouse; }

	//キーボード取得
	Keyboard& GeKeyboard() { return keyboard; }

	// ゲームパッド取得
	GamePad& GetGamePad(int padNum = 0) { return gamePads.at(padNum); }
	std::vector<GamePad>& GetGamePads() { return gamePads; }
private:
	static Input* instance;
	Mouse mouse;
	Keyboard keyboard;
	std::vector<GamePad> gamePads;
};
