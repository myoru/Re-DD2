#include "Slide.h"

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

Slide::Slide(Type a_type)
	:m_type(static_cast<int>(a_type))
{
}

Slide::~Slide()
{
}

void Slide::DeleteCharacter()
{
	// m_characters から m_removes に含まれているキャラクターを削除する
	m_characters.erase(
		std::remove_if(m_characters.begin(), m_characters.end(),
			[this](const std::shared_ptr<Character>& chara)
			{
				//削除リストに含まれていれば true
				return m_removes.contains(chara);
			}
		), m_characters.end());

	//破棄リストをクリア
	m_removes.clear();
}

bool Slide::Update(float a_elapsedTime, const Mouse& a_mouse)
{
	switch (static_cast<State>(m_state))
	{
	case State::StartUp:
		m_startUpTimer += a_elapsedTime;
		if (m_startUpTimer > m_startUpTime)
		{
			m_state = sc_i(State::Reading);
		}
		break;
	case State::Reading:
		if (a_mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			m_state = sc_i(State::ReadEnd);
		}
		break;
	case State::ReadEnd:
		m_readEndTimer += a_elapsedTime;
		if (m_readEndTimer >= m_readEndTime)
		{
			m_readEndTimer = m_readEndTime;
			if (a_mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				return true;
			}
		}
		break;
	}

	return false;
}

void Slide::Render(DirectX::XMFLOAT2 a_reviewLeftTopPos, DirectX::XMFLOAT2 a_reviewSize)
{
	//背景スプライト描画
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, a_reviewLeftTopPos, a_reviewSize);
	}

	int _i = 0;
	for (auto& _character : m_characters)
	{
		if (_i == m_characterIndex)
		{
			//Tool用の描画処理(選択中なのをわかりやすくする)
			_character->ToolOutLineRender(BasePoint::Center, { 0.75f,0.1f,0.3f,0.3f });
		}
		//キャラクター描画
		_character->ToolRender(BasePoint::Center);
		_i++;
	}
}
