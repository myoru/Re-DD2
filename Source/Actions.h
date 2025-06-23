#pragma once
#include <vector>
#include <memory>
#include "Action.h"
#include "AllBGM.h"

//static void MakeUpAction(std::vector<std::shared_ptr<Action>>& a_actions, Action::Data a_actionData)
//{
//	const std::string _name = a_actionData.name;
//	if (_name == "Vibe")
//	{
//		a_actions.emplace_back(std::make_shared<Action>(a_actionData));
//	}
//	if (_name == "Vibe")
//	{
//		a_actions.emplace_back(std::make_shared<Action>(a_actionData));
//	}
//}

class Vibe : public Action
{
public:
	Vibe() = default;
	Vibe(Action::Data a_data) :Action(a_data)
	{

	}
	~Vibe() {}

	void Excute(float a_elapsedTime)override
	{
		m_data.timer += a_elapsedTime;
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
CEREAL_REGISTER_TYPE(Vibe)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, Vibe)

class MusicStart : public Action
{
public:
	MusicStart() = default;
	MusicStart(Action::Data a_data) :Action(a_data) {}
	~MusicStart() {}

	void Excute(float a_elapsedTime)override
	{
		if (m_data.isEnd == true)return;
		AllBGM::GetInstance()->GetBGM(m_data.index)->Play(true);
		m_data.isEnd = true;
	}

	template<class T>
	void serialize(T& archive)
	{
		Action::serialize(archive);
	}
};
CEREAL_REGISTER_TYPE(MusicStart)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, MusicStart)

class MusicStop : public Action
{
public:
	MusicStop() = default;
	MusicStop(Action::Data a_data) :Action(a_data) {}
	~MusicStop() {}

	void Excute(float a_elapsedTime)override
	{
		if (m_data.isEnd == true)return;
		static float _volume = 1.0f;
		_volume -= a_elapsedTime;
		AllBGM::GetInstance()->GetBGM(m_data.index)->Volume(_volume);
		if (_volume < 0.0f)
		{
			AllBGM::GetInstance()->GetBGM(m_data.index)->Stop(true);
			m_data.isEnd = true;
		}
	}

	template<class T>
	void serialize(T& archive)
	{
		Action::serialize(archive);
	}
};
CEREAL_REGISTER_TYPE(MusicStop)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, MusicStop)