#pragma once
#include <vector>
#include <memory>
#include "Action.h"
#include "AllBGM.h"

static inline std::vector<std::string> actionStrList =
{
	reinterpret_cast<const char*>(u8"âÊñ êUìÆ"),
	reinterpret_cast<const char*>(u8"BGMçƒê∂"),
	reinterpret_cast<const char*>(u8"BGMí‚é~") };

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
		archive(cereal::base_class<Action>(this));
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

	void Enter()override
	{
		AllBGM::GetInstance()->GetBGM(m_data.index)->Play(true);
		m_data.isEnd = true;
	}

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<Action>(this));
	}
};
CEREAL_REGISTER_TYPE(MusicStart)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, MusicStart)

class AllMusicStop : public Action
{
public:
	AllMusicStop() = default;
	AllMusicStop(Action::Data a_data) :Action(a_data) {}
	~AllMusicStop() {}

	void Excute(float a_elapsedTime)override
	{
		bool _allBGMStopFlag = true;
		for (auto& _BGM : AllBGM::GetInstance()->GetBGMBox())
		{
			if (_BGM->IsPlaying())
			{
				float _beforeVolume = _BGM->GetVolume();
				float _afterVolume = _beforeVolume * 0.95f;
				if (_afterVolume < 0.05f)
				{
					_BGM->Stop();
				}
				else
				{
					_BGM->SetVolume(_afterVolume);
					_allBGMStopFlag = false;
				}
			}
		}
		if (_allBGMStopFlag)
		{
			m_data.isEnd = true;
		}
	}

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<Action>(this));
		archive(CEREAL_NVP(count));
	}

public:
	int count = 0;
	//if (MusicStop* musicStop = dynamic_cast<MusicStop*>(action)) 
	//{
	//	int c = musicStop->count;
	//	musicStop->count = 42;  // èëÇ´çûÇ›Ç‡OK
	//}
};
CEREAL_REGISTER_TYPE(AllMusicStop)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Action, AllMusicStop)