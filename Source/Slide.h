#pragma once
#include "Character.h"

class Slide
{
public:
	enum class Type;
	Slide(Type a_type = Type::Normal);
	//Slide(const Slide& slide);
	~Slide();

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

	std::vector<std::shared_ptr<Character>>& GetAppearingCharacters() { return m_appearingCharacters; }

public:
	//struct Action
	//{
	//	std::function<void()> function{};
	//	std::string functionStr{};
	//};

	//ƒXƒ‰ƒCƒh‚ÌŽí—Þ
	enum class Type
	{
		Normal,
	};
private:
	Type m_type = Type::Normal;
	std::vector<std::shared_ptr<Character>> m_appearingCharacters;
	//std::shared_ptr<Character> m_mainCharacter;
	std::shared_ptr<Sprite> m_backSpr;
	std::string m_text;
	//std::unordered_map<std::string, std::function<void()>> m_actions;
	//std::unordered_map<std::string, Action> m_actions;
	float m_mainTimer{};
};