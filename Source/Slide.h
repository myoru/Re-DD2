#pragma once
#include "Character.h"
#include "Action.h"

class Slide
{
public:
	enum class Type;
	Slide(Type a_type = Type::Normal);
	~Slide();

	void DeleteCharacter();

	void Render(DirectX::XMFLOAT2 a_reviewLeftTopPos,DirectX::XMFLOAT2 a_reviewSize);

	/*void ExcuteAllActions()
	{
		for (auto& [_key, _action] : m_actions)
		{
			m_actions[_key].function();
		}
	}
	void ExcuteAction(const std::string& a_key)
	{
		m_actions[a_key].function();
	}

	void AddAction(const std::string& a_key, std::function<void()> a_action)
	{
		m_actions[a_key].function = a_action;
	}
	void DeleteAction(const std::string& a_key)
	{
		m_actions.erase(a_key);
	}
	void DeleteAllAction(const std::string& a_key)
	{
		m_actions.clear();
	}*/

public:
	//ƒXƒ‰ƒCƒh‚ÌŽí—Þ
	enum class Type
	{
		Normal,
	};

	template<class T>
	void serialize(T& archive)
	{
		archive(
			CEREAL_NVP(m_type),
			CEREAL_NVP(m_characters),
			CEREAL_NVP(m_text),
			CEREAL_NVP(m_actions)
		);
	}
public:
	int m_type = static_cast<int>(Type::Normal);
	std::vector<std::shared_ptr<Character>> m_characters;
	std::set<std::shared_ptr<Character>> m_removes;
	int m_characterIndex = 0;
	std::shared_ptr<Sprite> m_backSpr;
	std::string m_backSprFilePath{};
	char m_inputBuffer[256] = {};
	std::string m_text;
	float m_mainTimer{};
	std::vector<std::shared_ptr<Action>> m_actions;
};