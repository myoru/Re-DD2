#include "Chapter.h"
#include "Graphics.h"
#include "StringConvert.h"
#include "Options.h"
#include "Input.h"
#include "AllAudio.h"

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

Chapter::Chapter()
{
	Graphics& _graphics = Graphics::Instance(); //RenderingStateなどを管理しているGraphicsのインスタンス取得
	ID3D11Device* _device = _graphics.GetDevice(); //Device取得
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext(); //DeviceContext取得

	m_slides.emplace_back(Slide());
	m_currentSlide = &m_slides[m_currentSlideIndex];
	m_slideshowEndSignboars = std::make_unique<SignBoard>(_device, _deviceContext, "Data\\Json\\SignBoard\\EndSlideshow.json");
	m_slideshowEndSignboars->jElements.text = "スライドショー 終了";
}

bool Chapter::Update(float a_elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	Mouse& _mouse = Input::Instance().GetMouse();

	Slide::State _returnState = m_currentSlide->Update(a_elapsedTime, _mouse);

	//m_currentSlide->Updateの返り値からSlideのステート(テキスト送りが終了したか など)を受け取り、適切な処理を行う
	switch (_returnState)
	{
	case Slide::State::Reading: //テキスト送り中
		break;
	case Slide::State::ReadEnd: //テキスト送り終了(クリックを押すと次のスライドに進める状態)
		break;
	case Slide::State::SlideMove: //クリックによって次のスライドへ進む入力がされた場合
		//ステートを初期状態に戻しておく(念のため)
		m_currentSlide->m_state = sc_i(Slide::State::Reading);
		//スライドのアクションとスライドが持つキャラクターのアクション終了処理を行う
		m_currentSlide->ActionsExit();
		//次のスライドがなければfalseを返す
		if (m_currentSlideIndex + 1 >= m_slides.size())
		{
			return false;
		}
		//次のスライドへと進む
		m_currentSlideIndex++;
		m_currentSlide = &m_slides[m_currentSlideIndex];
		//スライドのアクションとスライドが持つキャラクターのアクション開始処理を行う
		m_currentSlide->ActionsEnter();
		break;
	}

	//キャラクター更新処理
	CharactersUpdate(a_elapsedTime);

	//テキストウィンドウの更新処理を行う
	for (auto& _signBoard : m_signBoards)
	{
		_signBoard->ToolUpdate(a_elapsedTime, m_currentSlide->m_inputBuffer, m_currentSlide->m_drawableTextLen,
			XMFLOAT2{ 0.0f,0.0f }, XMFLOAT2{ _graphics.GetScreenWidth(),_graphics.GetScreenHeight() });
	}

	m_slideshowEndSignboars->Update(a_elapsedTime);

	return true;
}

void Chapter::ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize)
{
	Keyboard& _keyboard = Input::Instance().GeKeyboard();

	//キャラクター削除処理
	m_currentSlide->DeleteCharacter();
	//キャラクター更新処理
	ToolCharactersUpdate(a_elapsedTime, a_reviewScreenLeftTopPos, a_reviewScreenSize);

	//スライドの更新処理
	m_currentSlide->ToolUpdate(a_elapsedTime);

	//テキストウィンドウの更新処理を行う
	for (auto& _signBoard : m_signBoards)
	{
		_signBoard->ToolUpdate(a_elapsedTime, m_currentSlide->m_inputBuffer, m_currentSlide->m_drawableTextLen, a_reviewScreenLeftTopPos, a_reviewScreenSize);
	}
}

void Chapter::CharactersUpdate(float a_elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();

	for (auto& _character : m_currentSlide->m_characters)
	{
		_character->Update(a_elapsedTime, XMFLOAT2{ 0.0f,0.0f }, XMFLOAT2{ _graphics.GetScreenWidth(),_graphics.GetScreenHeight() });
	}
}

void Chapter::ToolCharactersUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize)
{
	for (auto& _character : m_currentSlide->m_characters)
	{
		_character->Update(a_elapsedTime, a_reviewScreenLeftTopPos, a_reviewScreenSize);
	}
}

void Chapter::StartSlideshow()
{
	for (auto& _slide : m_slides)
	{
		_slide.StartSlideshow();
	}
}

void Chapter::SlideshowRender()
{
	m_currentSlide->Render();

	if (m_signBoards.size())
	{
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->BoardRender();
		}
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->TextRender(true);
		}
	}
}

void Chapter::EndSlideshowRender()
{
	m_slideshowEndSignboars->Render();
}

void Chapter::ToolRender(DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize, bool a_isFullReview)
{
	m_currentSlide->ToolRender(a_reviewScreenLeftTopPos, a_reviewScreenSize);

	if (m_signBoards.size())
	{
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->BoardRender();
		}
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->TextRender(a_isFullReview);
		}
	}
}

void Chapter::EndSlideShow()
{
	for (auto& _BGM : AllAudio::GetInstance()->GetBGMBox())
	{
		if (_BGM->IsPlaying())
		{
			_BGM->Stop();
		}
	}

	for (auto& _slide : m_slides)
	{
		_slide.EndSlideShow();
	}
}
