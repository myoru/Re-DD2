#include "TestFacials1.h"

TestFacials1::TestFacials1()
{
	AddFacial("Dust", std::make_shared<Sprite>(L".\\Data\\Sprite\\Dust.png"));
	AddFacial("Punch", std::make_shared<Sprite>(L".\\Data\\Sprite\\Punch.png"));
	AddFacial("Kick", std::make_shared<Sprite>(L".\\Data\\Sprite\\Kick.png"));
	AddFacial("Slash", std::make_shared<Sprite>(L".\\Data\\Sprite\\Slash.png"));
	AddFacial("HighSlash", std::make_shared<Sprite>(L".\\Data\\Sprite\\HighSlash.png"));
}
