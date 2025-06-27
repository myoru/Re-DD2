#include "Chapter.h"
#include "Graphics.h"
#include "StringConvert.h"
#include "Options.h"
#include "Input.h"

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

void Chapter::Update(float a_elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	Mouse& _mouse = Input::Instance().GetMouse();

	if (m_currentSlide->Update(a_elapsedTime, _mouse))
	{
		m_currentSlideIndex++;
		m_currentSlide = &m_slides[m_currentSlideIndex];
		m_currentSlide->Update(a_elapsedTime, _mouse);
	}

	float _addTime = 0.0f;
	switch (Options::GetInstance()->GetTextReadSpeed())
	{
	case Options::TextReadSpeed::MostSlow:
		_addTime = a_elapsedTime * 10.0f; break;
	case Options::TextReadSpeed::Slow:
		_addTime = a_elapsedTime * 15.0f; break;
	case Options::TextReadSpeed::Normal:
		_addTime = a_elapsedTime * 23.0f; break;
	case Options::TextReadSpeed::Fast:
		_addTime = a_elapsedTime * 32.0f; break;
	case Options::TextReadSpeed::MostFast:
		_addTime = a_elapsedTime * 40.0f; break;
	}

	switch (static_cast<Slide::State>(m_currentSlide->m_state))
	{
	case Slide::State::StartUp:
		m_currentSlide->m_textTimer = 0.0f;
		break;
	case Slide::State::Reading:
		m_currentSlide->m_textTimer += _addTime;
		break;
	case Slide::State::ReadEnd:
		m_currentSlide->m_textTimer = 256.0f;
		break;
	}

	int _drawableTextLen = static_cast<int>(m_currentSlide->m_textTimer);

	if (m_currentSlide->m_characters.size())
	{
		for (auto& _slide : m_slides)
		{
			_slide.DeleteCharacter();
		}

		for (int i = 0, iEnd = m_currentSlide->m_characters.size(); i < iEnd; i++)
		{
			m_currentSlide->m_characters[i]->ToolUpdate(a_elapsedTime, XMFLOAT2{ 0.0f,0.0f }, XMFLOAT2{ _graphics.GetScreenWidth(),_graphics.GetScreenHeight() });
		}
	}

	if (m_signBoards.size())
	{
		//更新処理を行う
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			if (m_signBoards.at(i)->Update(a_elapsedTime, m_currentSlide->m_inputBuffer, _drawableTextLen, XMFLOAT2{ 0.0f,0.0f }, XMFLOAT2{ _graphics.GetScreenWidth(),_graphics.GetScreenHeight() }))
			{
				if (!m_textReadEndFlag)
				{
					m_currentSlide->m_textTimer = 0.0f;
				}
			}
		}
	}
}

void Chapter::ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize)
{
	Keyboard& _keyboard = Input::Instance().GeKeyboard();
	Mouse& _mouse = Input::Instance().GetMouse();


	if (m_textReadEndFlag)
	{
		if (_mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		{
			m_currentSlide->m_textTimer = 0.0f;
			m_textReadEndFlag = false;
			m_currentSlideIndex++;
			m_currentSlide = &m_slides[m_currentSlideIndex];
		}
	}
	else
	{
		if (_mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		{
			m_currentSlide->m_textTimer = 256.0f;
			m_textReadEndFlag = true;
		}
		else
		{
			float _addTime = 0.0f;
			switch (Options::GetInstance()->GetTextReadSpeed())
			{
			case Options::TextReadSpeed::MostSlow:
				_addTime = a_elapsedTime * 10.0f; break;
			case Options::TextReadSpeed::Slow:
				_addTime = a_elapsedTime * 15.0f; break;
			case Options::TextReadSpeed::Normal:
				_addTime = a_elapsedTime * 23.0f; break;
			case Options::TextReadSpeed::Fast:
				_addTime = a_elapsedTime * 32.0f; break;
			case Options::TextReadSpeed::MostFast:
				_addTime = a_elapsedTime * 40.0f; break;
			}
			m_currentSlide->m_textTimer += _addTime;
		}
	}

	int _drawableTextLen = static_cast<int>(m_currentSlide->m_textTimer);

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

	//看板(m_signBoard)が一つでも登録されていれば
	if (m_signBoards.size())
	{
		//更新処理を行う
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			if (m_signBoards.at(i)->Update(a_elapsedTime, m_currentSlide->m_inputBuffer, _drawableTextLen, a_reviewScreenLeftTopPos, a_reviewScreenSize))
			{
				if (!m_textReadEndFlag)
				{
					m_currentSlide->m_textTimer = 0.0f;
				}
			}
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