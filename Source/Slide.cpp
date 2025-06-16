#include "Slide.h"

Slide::Slide(Type a_type)
	:m_type(static_cast<int>(a_type))
{
}

Slide::~Slide()
{
}

void Slide::DeleteCharacter()
{
	m_characters.erase(
		std::remove_if(
			m_characters.begin(),
			m_characters.end(),
			[this](const std::shared_ptr<Character>& ch) 
			{
				return m_removes.contains(ch);
			}
		),
		m_characters.end()
	);

	//破棄リストをクリア
	m_removes.clear();
}

void Slide::Render(DirectX::XMFLOAT2 a_reviewLeftTopPos, DirectX::XMFLOAT2 a_reviewSize)
{
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, a_reviewLeftTopPos, a_reviewSize);
	}

	int _i = 0;
	for (auto& _character : m_characters)
	{
		if (_i == m_characterIndex)
		{
			_character->ToolOutLineRender(BasePoint::Center, { 0.75f,0.1f,0.3f,0.3f });
		}
		_character->ToolRender(BasePoint::Center);
		_i++;
	}
}
