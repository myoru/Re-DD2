#include "RectUI.h"

RectUI::RectUI(const wchar_t* a_fileName, BasePoint a_basePoint)
{
	sprite = std::make_unique<Sprite>(a_fileName, a_basePoint);
}
