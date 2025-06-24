#include "Chapter.h"

void Chapter::Update(float a_elapsedTime)
{
	m_currentSlide = &m_slides[m_currentSlideIndex];

	//m_currentSlide->

}

void Chapter::ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize)
{
	m_textTimer += a_elapsedTime;
	int _drawableTextLen = static_cast<int>(m_textTimer);

	if (m_currentSlide->m_characters.size())
	{
		for (auto& _slide : m_slides)
		{
			_slide.DeleteCharacter();
		}

		for (int i = 0, iEnd = m_currentSlide->m_characters.size(); i < iEnd; i++)
		{
			m_currentSlide->m_characters[i]->ToolUpdate(a_elapsedTime, a_reviewScreenLeftTopPos, a_reviewScreenSize);
		}
	}

	//ŠÅ”Â(m_signBoard)‚ªˆê‚Â‚Å‚à“o˜^‚³‚ê‚Ä‚¢‚ê‚Î
	if (m_signBoards.size())
	{
		//XVˆ—‚ğs‚¤
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->Update(a_elapsedTime, m_currentSlide->m_inputBuffer, _drawableTextLen, a_reviewScreenLeftTopPos, a_reviewScreenSize);
		}
	}
}

void Chapter::Render()
{

}

void Chapter::ToolRender(DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize, bool a_isFullReview)
{
	m_currentSlide->Render(a_reviewScreenLeftTopPos, a_reviewScreenSize);

	if (m_signBoards.size())
	{
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->BoardRender();
		}
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->TextRender(a_isFullReview);
		}
	}
}