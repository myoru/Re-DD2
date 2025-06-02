#pragma once
#include "Mouse.h"
#include "Keyboard.h"
#include "GamePad.h"
#include <vector>

#define PAD_MAX 5

// �C���v�b�g
class Input
{
public:
	Input(HWND hWnd, int padNum = 0);
	~Input() {}

public:
	// �C���X�^���X�擾
	static Input& Instance() { return *instance; }

	// �X�V����
	void Update(int padNum = 0);

	// �}�E�X�擾
	Mouse& GetMouse() { return mouse; }

	//�L�[�{�[�h�擾
	Keyboard& GeKeyboard() { return keyboard; }

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad(int padNum = 0) { return gamePads.at(padNum); }
	std::vector<GamePad>& GetGamePads() { return gamePads; }
private:
	static Input* instance;
	Mouse mouse;
	Keyboard keyboard;
	std::vector<GamePad> gamePads;
};
