#pragma once
#include "Character.h"
#include "SlideAction.h"
#include "Input.h"

class Slide
{
public:
	enum class Type;
	enum class State;
	Slide(Type a_type = Type::Normal);
	~Slide();

	//キャラクター削除関数  ※Tool用
	void DeleteCharacter();
	//更新処理
	State Update(float a_elapsedTime, const Mouse& a_mouse);
	bool ToolUpdate(float a_elapsedTime);
	void StartSlideshow();
	void EndSlideShow();

	void AddAction(int a_actionIndex);
	void ActionsEnter();
	bool ActionsExecute(float a_elapsedTime);
	void ActionsExit();
	bool EternalActionsExecute(float a_elapsedTime);
	//描画関数
	void Render();
	void ToolRender(DirectX::XMFLOAT2 a_reviewLeftTopPos,DirectX::XMFLOAT2 a_reviewSize);
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
	int m_state = static_cast<int>(State::Reading); //ステート
	std::vector<std::shared_ptr<Character>> m_characters; //スライドに登場するキャラクター
	std::set<std::shared_ptr<Character>> m_removes; //キャラクターの削除リスト ※Tool用
	int m_characterIndex = 0; //どのキャラクターを選んでいるか ※おそらくTool用
	std::shared_ptr<Sprite> m_backSpr; //背景スプライト
	std::string m_backSprFilePath{}; //背景スプライトのファイルパス  ※Json
	char m_inputBuffer[256] = {}; //テキストウィンドウ用文字列
	std::string m_text; //テキストウィンドウ用文字列  ※Json
	float m_mainTimer{}; //スライドごとのタイマー
	int m_drawableTextLen = 0; //何文字表示できるか
	float m_textReadTimer = 0.0f; //テキスト送りに使うタイマー(int型にcastして使う)
	float m_readEndTimer = 0.0f; // テキストを送り終わってから少しの間クリックによる次スライドへの移行をさせないようにするためのタイマー
	std::vector<std::shared_ptr<SlideAction>> m_actions; //スライドで行うアクションリスト(例,BGM再生・停止)
};