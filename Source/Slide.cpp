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
	// m_characters ���� m_removes �Ɋ܂܂�Ă���L�����N�^�[���폜����
	m_characters.erase(
		std::remove_if(m_characters.begin(), m_characters.end(),
			[this](const std::shared_ptr<Character>& chara)
			{
				//�폜���X�g�Ɋ܂܂�Ă���� true
				return m_removes.contains(chara);
			}
		), m_characters.end());

	//�j�����X�g���N���A
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
	//�w�i�X�v���C�g�`��
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, a_reviewLeftTopPos, a_reviewSize);
	}

	int _i = 0;
	for (auto& _character : m_characters)
	{
		if (_i == m_characterIndex)
		{
			//Tool�p�̕`�揈��(�I�𒆂Ȃ̂��킩��₷������)
			_character->ToolOutLineRender(BasePoint::Center, { 0.75f,0.1f,0.3f,0.3f });
		}
		//�L�����N�^�[�`��
		_character->ToolRender(BasePoint::Center);
		_i++;
	}
}
