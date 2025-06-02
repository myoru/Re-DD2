#include "TestFacials2.h"

TestFacials2::TestFacials2()
{
	AddFacial("RB",std::make_shared<Sprite>(L".\\Data\\Sprite\\RB.png"));
	AddFacial("RT",std::make_shared<Sprite>(L".\\Data\\Sprite\\RT.png"));
	AddFacial("LB",std::make_shared<Sprite>(L".\\Data\\Sprite\\LB.png"));
}
