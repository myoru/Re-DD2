#pragma once
#include <vector>
#include <memory>
#include "Audio.h"
#include "Singleton.h"

class AllBGM : public Singleton<AllBGM>
{
public:
	AllBGM() :Singleton() {}
	~AllBGM()override {}

	bool InitializeElements()override;

	std::vector<std::shared_ptr<Audio>> GetBGMBox() { return BGMBox; }
	std::vector<std::shared_ptr<Audio>> GetSEBox() { return SEBox; }
	std::shared_ptr<Audio> GetBGM(int a_index) { return BGMBox[a_index]; }
	std::shared_ptr<Audio> GetSE(int a_index) { return SEBox[a_index]; }
private:
	std::vector<std::shared_ptr<Audio>> BGMBox;
	std::vector<std::shared_ptr<Audio>> SEBox;
};