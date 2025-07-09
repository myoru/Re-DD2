#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Audio.h"
#include "Singleton.h"

class AllAudio : public Singleton<AllAudio>
{
public:
	AllAudio() :Singleton() {}
	~AllAudio()override {}

	bool InitializeElements()override
	{
		BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Title.wav"));
		BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Select.wav"));
		BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\CharacterSelect.wav"));
		BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Battle.wav"));

		return true;
	}

	std::vector<std::shared_ptr<Audio>> GetBGMBox() { return BGMBox; }
	std::vector<std::shared_ptr<Audio>> GetSEBox() { return SEBox; }
	std::shared_ptr<Audio> GetBGM(int a_index) { return BGMBox[a_index]; }
	std::shared_ptr<Audio> GetSE(int a_index) { return SEBox[a_index]; }
private:
	std::vector<std::shared_ptr<Audio>> BGMBox;
	std::vector<std::shared_ptr<Audio>> SEBox;

public:
	static inline std::vector<std::string> BGMStrList =
	{
		reinterpret_cast<const char*>(u8"タイトル"),
		reinterpret_cast<const char*>(u8"モード選択"),
		reinterpret_cast<const char*>(u8"キャラクターセレクト"),
		reinterpret_cast<const char*>(u8"戦闘"),
	};
};