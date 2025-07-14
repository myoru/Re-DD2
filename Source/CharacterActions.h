#pragma once
#include <vector>
#include <memory>
#include "CharacterAction.h"
#include "AllAudio.h"
#include "Mathf.h"
#include "ImGuiPack.h"
#include "Sprite.h"

//class Character;

static inline std::vector<std::string> characterActionStrList =
{
	reinterpret_cast<const char*>(u8"à⁄ìÆ"),
	reinterpret_cast<const char*>(u8"SEçƒê∂"),
	reinterpret_cast<const char*>(u8"êUìÆ")
};

enum class CharacterActionList
{
	Move,
	SEPlay,
	Shake,
};

class Move : public CharacterAction
{
public:
	enum class Mode
	{
		Start,
		Goal,
	};

	Move() = default;
	Move(CharacterAction::Data a_data) :CharacterAction(a_data) { Initialize(); }
	~Move() {}

	void Initialize()override;
	void Finalize()override;
	void Enter()override;
	bool Execute(float a_elapsedTime)override;
	void Exit()override;
	void ShowExecute()override;
	void ToolUpdate(float a_elapsedTime)override;

	DirectX::XMMATRIX GuizmoTranslate()override;
	void GuizmoDecompose(DirectX::XMMATRIX a_taransform)override;
	void UiRender(DirectX::XMFLOAT2 a_screenLeftTopPos, DirectX::XMFLOAT2 a_screenSize)override;

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<CharacterAction>(this));
		archive(CEREAL_NVP(m_startNormalizePos),
			CEREAL_NVP(m_goalNormalizePos));
	}

	DirectX::XMFLOAT2 m_startNormalizePos = {};
	DirectX::XMFLOAT2 m_goalNormalizePos = { 0.5f,0.5f };

private:
	std::unique_ptr<Sprite> m_whiteSpr;
};
CEREAL_REGISTER_TYPE(Move)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CharacterAction, Move)

class SEStart : public CharacterAction
{
public:
	SEStart() = default;
	SEStart(CharacterAction::Data a_data) :CharacterAction(a_data) {}
	~SEStart() {}

	void Enter()override;

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<CharacterAction>(this));
	}
};
CEREAL_REGISTER_TYPE(SEStart)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CharacterAction, SEStart)

class Shake : public CharacterAction
{
public:
	enum class Direction
	{
		Horizontal,
		Vertical,
		Random,
	};

	Shake() = default;
	Shake(CharacterAction::Data a_data) :CharacterAction(a_data) {}
	~Shake() {}

	void Enter()override;

	bool Execute(float a_elapsedTime)override;

	template<class T>
	void serialize(T& archive)
	{
		archive(cereal::base_class<CharacterAction>(this));
	}

	DirectX::XMFLOAT2 m_beginCharacterNormalizewPosition = {};
};
CEREAL_REGISTER_TYPE(Shake)
CEREAL_REGISTER_POLYMORPHIC_RELATION(CharacterAction, Shake)

//class MusicStart : public CharacterAction
//{
//public:
//	MusicStart() = default;
//	MusicStart(SlideAction::Data a_data) :SlideAction(a_data) {}
//	~MusicStart() {}
//
//	void Enter()override
//	{
//		AllBGM::GetInstance()->GetBGM(m_data.index)->Play(true);
//		m_data.isEnd = true;
//	}
//
//	template<class T>
//	void serialize(T& archive)
//	{
//		archive(cereal::base_class<SlideAction>(this));
//	}
//};
//CEREAL_REGISTER_TYPE(MusicStart)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SlideAction, MusicStart)
//
//class AllMusicStop : public CharacterAction
//{
//public:
//	AllMusicStop() = default;
//	AllMusicStop(SlideAction::Data a_data) :SlideAction(a_data) {}
//	~AllMusicStop() {}
//
//	void Excute(float a_elapsedTime)override
//	{
//		bool _allBGMStopFlag = true;
//		for (auto& _BGM : AllBGM::GetInstance()->GetBGMBox())
//		{
//			if (_BGM->IsPlaying())
//			{
//				float _beforeVolume = _BGM->GetVolume();
//				float _afterVolume = _beforeVolume * 0.95f;
//				if (_afterVolume < 0.05f)
//				{
//					_BGM->Stop();
//				}
//				else
//				{
//					_BGM->SetVolume(_afterVolume);
//					_allBGMStopFlag = false;
//				}
//			}
//		}
//		if (_allBGMStopFlag)
//		{
//			m_data.isEnd = true;
//		}
//	}
//
//	template<class T>
//	void serialize(T& archive)
//	{
//		archive(cereal::base_class<SlideAction>(this));
//		archive(CEREAL_NVP(count));
//	}
//
//public:
//	int count = 0;
//	//if (MusicStop* musicStop = dynamic_cast<MusicStop*>(action)) 
//	//{
//	//	int c = musicStop->count;
//	//	musicStop->count = 42;  // èëÇ´çûÇ›Ç‡OK
//	//}
//};
//CEREAL_REGISTER_TYPE(AllMusicStop)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(SlideAction, AllMusicStop)