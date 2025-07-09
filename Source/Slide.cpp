#include "Slide.h"
#include "Graphics.h"
#include "Options.h"
#include "StringConvert.h"
#include "SlideActions.h"

#define TextReadWaitTime 0.25f
#define NextSlideMoveWaitTime 0.15f

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
	// m_characters から m_removes に含まれているキャラクターを削除する
	m_characters.erase(
		std::remove_if(m_characters.begin(), m_characters.end(),
			[this](const std::shared_ptr<Character>& chara)
			{
				//削除リストに含まれていれば true
				return m_removes.contains(chara);
			}
		), m_characters.end());

	//破棄リストをクリア
	m_removes.clear();
}

Slide::State Slide::Update(float a_elapsedTime, const Mouse& a_mouse)
{
	m_mainTimer += a_elapsedTime; //メインのタイマーを加算する(用途不明)
	switch (static_cast<State>(m_state))
	{
	case State::Reading:
	{
		//スライドのアクション と キャラクターのアクションが全ての終了しているかのフラグ
		bool _actionEndCheck = ActionsExecute(a_elapsedTime);

		//テキスト送り用のタイマー加算(テキストの最大表示数より小さい時のみ加算)
		if (m_textReadTimer < 256.0f)
		{
			m_textReadTimer += a_elapsedTime;
		}
		//テキストウィンドウに何文字表示出来るかの値をテキスト送り用タイマーを基に計算する。
		m_drawableTextLen = sc_i(m_textReadTimer * Options::GetInstance()->GetTextReadSpeedValue());

		//テキストを強制的に全表示にさせるのに数秒の時間は待機してもらうよう制御する
		if (m_textReadTimer >= TextReadWaitTime)
		{
			int _textSize = countUTF8Chars(m_inputBuffer);
			//もし、クリックによってテキスト強制全表示の入力が行われたら、
			if (a_mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				//テキストを最大表示数分表示できるように値を変更する
				m_textReadTimer = 256.0f;
				m_drawableTextLen = _textSize;

				if (EternalActionsExecute(a_elapsedTime)) //まだ終了していないスライドのアクション と キャラクターのアクションがあれば、終了になるまで実行する。
				{
					m_state = sc_i(State::ReadEnd); //全てのテキストを表示している状態に変更する
				}
			}
			//もし、テキストが全表示されており、スライドのアクション と キャラクターのアクションが全て終了していたなら、
			else if (_textSize < m_drawableTextLen && _actionEndCheck)
			{
				//テキストを最大表示数分表示できるように値を変更する(念のため)
				m_drawableTextLen = _textSize;
				m_textReadTimer = 256.0f;
				m_state = sc_i(State::ReadEnd); //全てのテキストを表示している状態に変更する。
			}
		}
	}
	break;
	case State::ReadEnd:
		//テキストが全表示されてから使用するタイマーを加算する。
		m_readEndTimer += a_elapsedTime;
		if (m_readEndTimer >= NextSlideMoveWaitTime)
		{
			//もし、クリックによって、次のスライドに進む入力がされたら、
			if (a_mouse.GetButtonDown() & Mouse::BTN_LEFT)
			{
				m_state = sc_i(State::SlideMove); //次のスライドに進む状態に変数する。
				//制御用変数を初期化
				m_mainTimer = 0.0f;
				m_textReadTimer = 0.0f;
				m_readEndTimer = 0.0f;
			}
		}
		break;
	case State::SlideMove:
		//制御用変数を初期化
		m_mainTimer = 0.0f;
		m_textReadTimer = 0.0f;
		m_readEndTimer = 0.0f;
		break;
	}

	return static_cast<State>(m_state); //スライドの状態をChapterに返すことで進行させる。
}

bool Slide::ToolUpdate(float a_elapsedTime)
{
	m_mainTimer += a_elapsedTime;
	m_textReadTimer += a_elapsedTime;
	m_drawableTextLen = sc_i(m_textReadTimer * Options::GetInstance()->GetTextReadSpeedValue());

	if (countUTF8Chars(m_inputBuffer) <= m_drawableTextLen)
	{
		m_readEndTimer += a_elapsedTime;
		if (m_readEndTimer >= 3.0f)
		{
			m_textReadTimer = 0.0f;
			m_drawableTextLen = 0;
			m_readEndTimer = 0.0f;
			return true;
		}
	}

	for (auto& _character : m_characters)
	{
		for (auto& _action : _character->m_actions)
		{
			_action->ToolUpdate(a_elapsedTime);
		}
	}

	return false;
}

void Slide::StartSlideshow()
{
	m_state = sc_i(State::Reading);
	m_textReadTimer = 0.0f;
	m_drawableTextLen = 0;
	m_readEndTimer = 0.0f;

	////スライドアクション初期化処理
	//for (auto& _action : m_actions)
	//{
	//	_action->ini
	//}

	//キャラクターのスライドショー開始のための準備処理
	for (auto& _character : m_characters)
	{
		_character->StartSlideshow();
	}
}

void Slide::EndSlideShow()
{
	m_state = sc_i(State::Reading);
	m_textReadTimer = 0.0f;
	m_drawableTextLen = 0;
	m_readEndTimer = 0.0f;

	for (auto& _character : m_characters)
	{
		_character->EndSlideShow();
	}
}

void Slide::AddAction(int a_actionIndex)
{
	switch (a_actionIndex)
	{
	case 0:
		m_actions.emplace_back(std::make_shared<Vibe>(SlideAction::Data{ "Vibe" }));
		break;
	case 1:
		m_actions.emplace_back(std::make_shared<BGMOperate>(SlideAction::Data{ "BGMOperate",0 }));
		break;
	case 2:
		m_actions.emplace_back(std::make_shared<BGMFade>(SlideAction::Data{ "BGMFade",0 }));
		break;
	}
}

void Slide::ActionsEnter()
{
	//スライドのアクションの開始処理実行
	for (auto _slideAction : m_actions)
	{
		_slideAction->Enter();
	}

	//キャラクターのアクションの開始処理実行
	for (auto _character : m_characters)
	{
		_character->ActionsEnter();
	}
}

bool Slide::ActionsExecute(float a_elapsedTime)
{
	bool _endCheck = true;

	//スライドのアクションの更新処理実行
	for (auto _slideAction : m_actions)
	{
		if (!_slideAction->Execute(a_elapsedTime))
		{
			_endCheck = false;
		}
	}

	//キャラクターのアクションの更新処理実行
	for (auto _character : m_characters)
	{
		if (!_character->ActionsExecute(a_elapsedTime))
		{
			_endCheck = false;
		}
	}

	return _endCheck;
}

void Slide::ActionsExit()
{
	//スライドのアクションの終了処理実行
	for (auto _slideAction : m_actions)
	{
		_slideAction->Exit();
	}

	//キャラクターのアクションの終了処理実行
	for (auto _character : m_characters)
	{
		_character->ActionsExit();
	}
}

bool Slide::EternalActionsExecute(float a_elapsedTime)
{
	bool _endCheck = false;

	while (!_endCheck)
	{
		_endCheck = true;
		//スライドのアクションの更新処理実行
		for (auto _slideAction : m_actions)
		{
			if (!_slideAction->Execute(a_elapsedTime))
			{
				_endCheck = false;
			}
		}

		//キャラクターのアクションの更新処理実行
		for (auto _character : m_characters)
		{
			for (auto _characterAction : _character->m_actions)
			{
				if (!_characterAction->Execute(a_elapsedTime))
				{
					_endCheck = false;
				}
			}
		}
	}

	return true;

}

void Slide::Render()
{
	Graphics& _graphics = Graphics::Instance();

	//背景スプライト描画
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, { 0.0f,0.0f }, { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() });
	}

	for (auto& _character : m_characters)
	{
		//キャラクター描画
		_character->Render(BasePoint::Center);
	}
}

void Slide::ToolRender(DirectX::XMFLOAT2 a_reviewLeftTopPos, DirectX::XMFLOAT2 a_reviewSize)
{
	//背景スプライト描画
	if (m_backSpr)
	{
		m_backSpr->Render(BasePoint::LeftTop, a_reviewLeftTopPos, a_reviewSize);
	}

	int _i = 0;
	for (auto& _character : m_characters)
	{
		if (_i == m_characterIndex)
		{
			//Tool用の描画処理(選択中なのをわかりやすくする)
			_character->ToolOutLineRender(BasePoint::Center, { 0.75f,0.1f,0.3f,0.3f });
		}
		//キャラクター描画
		_character->Render(BasePoint::Center);

		if (_character->m_actions.size())
		{
			for (auto& _action : _character->m_actions)
			{
				_action->UiRender(a_reviewLeftTopPos, a_reviewSize);
			}
		}
		_i++;
	}
}
