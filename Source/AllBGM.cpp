#include "AllBGM.h"

bool AllBGM::InitializeElements()
{
	BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Title.wav"));
	BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Select.wav"));
	BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\CharacterSelect.wav"));
	BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Battle.wav"));

	return true;
}
