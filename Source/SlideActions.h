#pragma once
#include <vector>
#include <memory>
#include "ImGuiPack.h"
#include "SlideAction.h"
#include "AllAudio.h"


static inline std::vector<std::string> slideActionStrList =
{
	reinterpret_cast<const char*>(u8"画面振動"),
	reinterpret_cast<const char*>(u8"BGM操作"),
	reinterpret_cast<const char*>(u8"BGMフェード")
};

enum class SlideActionList
{
	WindowShake,
	BGMOperate,
	BGMFade,
};

class Vibe : public SlideAction
{
public:
	Vibe() = default;
	Vibe(SlideAction::Data a_data) :SlideAction(a_data) {}
	~Vibe() {}

	bool Execute(float a_elapsedTime)override
	{
		if (m_data.isEnd) return m_data.isEnd;

		m_data.timer += a_elapsedTime;
		m_data.index++;
		if (m_data.index >= 500)
		{
			m_data.isEnd = true;
		}

		return m_data.isEnd;
	}

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<SlideAction>(this));
	}
};
CEREAL_REGISTER_TYPE(Vibe)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SlideAction, Vibe)

class BGMOperate : public SlideAction
{
public:
	BGMOperate() = default;
	BGMOperate(SlideAction::Data a_data) :SlideAction(a_data) {}
	~BGMOperate() {}

	static inline std::vector<std::string> ModeStrList =
	{
		reinterpret_cast<const char*>(u8"再生"),
		reinterpret_cast<const char*>(u8"停止"),
		reinterpret_cast<const char*>(u8"再生＆停止"),
	};

	enum class Mode
	{
		Play,
		Stop,
		PlayAndStop,
	};

	void OperateData()override
	{
		if (ImGui::TreeNodeEx(slideActionStrList[static_cast<int>(SlideActionList::BGMOperate)].c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			std::vector<const char*> _cStrItems;

			// 一時的に const char* の配列を作成（ImGui::Combo 用）
			for (const auto& str : ModeStrList)
			{
				_cStrItems.push_back(str.c_str());
			}
			ImGui::Combo("Mode", &m_data.mode, _cStrItems.data(), static_cast<int>(_cStrItems.size()));
			ImGui::TreePop();
		}
	}

	void Enter()override
	{
		if (m_data.mode != static_cast<int>(Mode::Stop))
		{
			AllAudio::GetInstance()->GetBGM(m_data.index)->Play(true);
			m_data.isEnd = true;
		}
	}

	void Exit()override
	{
		if (m_data.mode != static_cast<int>(Mode::Play))
		{
			AllAudio::GetInstance()->GetBGM(m_data.index)->Stop();
			m_data.isEnd = true;
		}
	}

	void ShowEnter()override
	{
		if (m_data.mode != static_cast<int>(Mode::Stop))
		{
			std::string label = slideActionStrList[static_cast<int>(SlideActionList::BGMOperate)] + "##bgmHeader" + std::to_string(static_cast<int>(Attribute::Enter));
			if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				std::vector<const char*> _cStrItems;

				// 一時的に const char* の配列を作成（ImGui::Combo 用）
				for (const auto& str : AllAudio::BGMStrList)
				{
					_cStrItems.push_back(str.c_str());
				}
				ImGui::Combo("BGM", &m_data.index, _cStrItems.data(), static_cast<int>(_cStrItems.size()));
			}
		}
	}

	void ShowExit()override
	{
		if (m_data.mode != static_cast<int>(Mode::Play))
		{
			std::string label = slideActionStrList[static_cast<int>(SlideActionList::BGMOperate)] + "##bgmHeader" + std::to_string(static_cast<int>(Attribute::Exit));
			if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				std::vector<const char*> _cStrItems;

				// 一時的に const char* の配列を作成（ImGui::Combo 用）
				for (const auto& str : AllAudio::BGMStrList)
				{
					_cStrItems.push_back(str.c_str());
				}
				ImGui::Combo("BGM", &m_data.index, _cStrItems.data(), static_cast<int>(_cStrItems.size()));
			}
		}
	}

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<SlideAction>(this));
	}
};
CEREAL_REGISTER_TYPE(BGMOperate)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SlideAction, BGMOperate)

class BGMFade : public SlideAction
{
public:
	BGMFade() = default;
	BGMFade(SlideAction::Data a_data) :SlideAction(a_data) {}
	~BGMFade() {}

	bool Execute(float a_elapsedTime)override
	{
		if (m_data.isEnd) return m_data.isEnd;

		bool _allBGMStopFlag = true;
		for (auto& _BGM : AllAudio::GetInstance()->GetBGMBox())
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

		return m_data.isEnd;
	}

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<SlideAction>(this));
		archive(CEREAL_NVP(count));
	}

public:
	int count = 0;
	//if (MusicStop* musicStop = dynamic_cast<MusicStop*>(action)) 
	//{
	//	int c = musicStop->count;
	//	musicStop->count = 42;  // 書き込みもOK
	//}
};
CEREAL_REGISTER_TYPE(BGMFade)
CEREAL_REGISTER_POLYMORPHIC_RELATION(SlideAction, BGMFade)