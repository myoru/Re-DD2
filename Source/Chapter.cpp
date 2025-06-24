#include "Chapter.h"

void Chapter::Update(float a_elapsedTime)
{
	m_currentSlide = &m_slides[m_currentSlideIndex];
}
