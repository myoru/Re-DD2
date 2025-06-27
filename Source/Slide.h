#pragma once
#include "Character.h"
#include "Action.h"
#include "Input.h"

class Slide
{
public:
	enum class Type;
	Slide(Type a_type = Type::Normal);
	~Slide();

	//キャラクター削除関数  ※Tool用
	void DeleteCharacter();
	//更新処理
	bool Update(float a_elapsedTime, const Mouse& a_mouse);
	//描画関数
	void Render(DirectX::XMFLOAT2 a_reviewLeftTopPos,DirectX::XMFLOAT2 a_reviewSize);
public:
	//スライドの種類
	enum class Type
	{
		Normal,
		TextDrawName,
		TextNotName,
		NotText,
	};

	enum class State
	{
		StartUp,
		Reading,
		ReadEnd,
		SlideMove,
	};

	template<class T>
	void serialize(T& archive)
	{
		archive(
			CEREAL_NVP(m_type),
			CEREAL_NVP(m_characters),
			CEREAL_NVP(m_text),
			CEREAL_NVP(m_actions),
			CEREAL_NVP(m_backSprFilePath)
		);
	}
public:
	int m_type = static_cast<int>(Type::Normal); //スライドのタイプ
	int m_state = static_cast<int>(State::StartUp); //ステート
	std::vector<std::shared_ptr<Character>> m_characters; //スライドに登場するキャラクター
	std::set<std::shared_ptr<Character>> m_removes; //キャラクターの削除リスト ※Tool用
	int m_characterIndex = 0; //どのキャラクターを選んでいるか ※おそらくTool用
	std::shared_ptr<Sprite> m_backSpr; //背景スプライト
	std::string m_backSprFilePath{}; //背景スプライトのファイルパス  ※Json
	char m_inputBuffer[256] = {}; //テキストウィンドウ用文字列
	std::string m_text; //テキストウィンドウ用文字列  ※Json
	float m_mainTimer{}; //スライドごとのタイマー
	float m_textTimer = 0.0f; //テキスト送りに使うタイマー(int型にcastして使う)
	std::vector<std::shared_ptr<Action>> m_actions; //スライドで行うアクションリスト(例,BGM再生・停止)
	float m_startUpTimer = 0.0f;
	float m_startUpTime = 0.35f;
	float m_readEndTimer = 0.0f;
	float m_readEndTime = 0.2f;
};