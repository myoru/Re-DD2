#include "AllBGM.h"

bool AllBGM::InitializeElements()
{
	BGMBox.emplace_back(std::make_shared<Audio>(L".\\Data\\Audio\\BGM\\Battle.wav"));

	return true;
}
