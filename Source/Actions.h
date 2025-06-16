#pragma once
#include <vector>
#include <memory>
#include "Action.h"

static void MakeUpAction(std::vector<std::shared_ptr<Action>>& a_actions, Action::Data a_actionData)
{
	const std::string _name = a_actionData.name;
	if (_name == "Vibe")
	{
		a_actions.emplace_back(std::make_shared<Action>(a_actionData));
	}
	if (_name == "Vibe")
	{
		a_actions.emplace_back(std::make_shared<Action>(a_actionData));
	}
}

class Vibe : public Action
{
public:
	Vibe() = default;
	Vibe(Action::Data a_data) :Action(a_data) {}
	~Vibe() {}

	void Excute(float a_elapsedTime)override
	{
		if (m_data.isEnd == true)return;
		m_data.index++;
		if (m_data.index >= 500)
		{
			m_data.isEnd = true;
		}
	}

	template<class T>
	void serialize(T& archive)
	{
		Action::serialize(archive);
	}
};

class MusicStart : public Action
{
public:
	MusicStart() = default;
	MusicStart(Action::Data a_data) :Action(a_data) {}
	~MusicStart() {}

	void Excute(float a_elapsedTime)override
	{
		if (m_data.isEnd == true)return;
		m_data.index--;
		if (m_data.index <= -500)
		{
			m_data.isEnd = true;
		}
	}

	template<class T>
	void serialize(T& archive)
	{
		Action::serialize(archive);
	}
};

// Register DerivedClassOne
CEREAL_REGISTER_TYPE(Vibe);
CEREAL_REGISTER_TYPE(MusicStart);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, Vibe)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, MusicStart)
