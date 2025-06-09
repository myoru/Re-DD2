#include "Slide.h"

Slide::Slide(Type a_type)
	:m_type(static_cast<int>(a_type))
{
}

//Slide::Slide(const Slide& slide):
//m_type(slide.m_type),m_text(slide.m_text),m_backSpr(slide.m_backSpr),m_appearingCharacters(std::move(slide.m_appearingCharacters))
//{
//
//}

Slide::~Slide()
{
}

void Slide::DeleteCharacter()
{
	////”jŠüˆ—
	//for (auto& _remove : m_removes)
	//{
	//	auto _it = std::find(m_characters.begin(), m_characters.end(), _remove);

	//	if (_it != m_characters.end())
	//	{
	//		m_characters.erase(_it);
	//	}

	//	//’eŠÛ‚Ì”jŠüˆ—
	//	delete _remove;
	//}
	
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

	//”jŠüƒŠƒXƒg‚ğƒNƒŠƒA
	m_removes.clear();
}

void Slide::Render(DirectX::XMFLOAT2 a_reviewLeftTopPos, DirectX::XMFLOAT2 a_reviewSize)
{
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, a_reviewLeftTopPos, a_reviewSize);
	}

	for (auto& _character : m_characters)
	{
		int _facialIndex = _character->facialIndex;
		_character->ToolRender(_facialIndex, BasePoint::Center);
	}
}
