#include <filesystem>
#include "Graphics.h"
#include "ImGuiPack.h"
#ifdef USE_IMGUI
#include "../External/imgui/ImGuizmo.h"
#endif
#include "SceneTool.h"
#include "Dialog.h"
#include "Input.h"
#include "StringConvert.h"
#include "AllFacialSet.h"
#include "SlideActions.h"
#include "CharacterActions.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "Options.h"

#define CharactersWindowRate 3.0f / 7.0f

using namespace DirectX;

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

void SceneTool::Initialize()
{
	using SeparateType = ScreenSeparateLine::SeparateType;
	Graphics& _graphics = Graphics::Instance(); //RenderingStateなどを管理しているGraphicsのインスタンス取得
	ID3D11Device* _device = _graphics.GetDevice(); //Device取得
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext(); //DeviceContext取得
	auto _allFacIns = AllFacialSet::GetInstance(); //シングルトン化されている全表情差分のインスタンス取得
	auto _allFacialSet = _allFacIns->GetAllFacialSet(); //全表情差分のリスト取得

	//Scene識別インデックス設定
	Scene::SetIndex(sc_i(Scene::Index::Event));

	////定数バッファ
	m_cb_fade->data.Initialize(5.0f, 1.0f, 1, 1, 0);

	//スプライトをロード
	m_spriteBox.emplace_back(std::make_unique<Sprite>(nullptr, BasePoint::LeftTop));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\Number.png"));

	//矩形UI初期化
	m_rectUIs["LTriangle"] = std::make_unique<RectUI>(L".\\Data\\Sprite\\SlideTriangle.png");
	m_rectUIs["RTriangle"] = std::make_unique<RectUI>(L".\\Data\\Sprite\\SlideTriangle.png");
	m_rectUIs["Add"] = std::make_unique<RectUI>(L".\\Data\\Sprite\\Add.png", BasePoint::RightTop);
	m_rectUIs["DustBox"] = std::make_unique<RectUI>(L".\\Data\\Sprite\\DustBox.png", BasePoint::RightBottom);

	//レビュー画面のパラメーター初期化
	m_reviewScreenPos = { (screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) + (screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) / 2.0f),
		(screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]) / 2.0f };
	m_reviewScreenAspectRate = m_reviewScreenNormalSize.y / m_reviewScreenNormalSize.x;
	m_reviewScreenSize.x = (screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)])) * 0.9f;
	m_reviewScreenSize.y = m_reviewScreenSize.x * m_reviewScreenAspectRate;

	//チャプター初期化
	m_chapter = std::make_unique<Chapter>();

#if 1
	std::string _input_json_name = "./Data/Json/";
	_input_json_name += "/Chapter/";
	_input_json_name += "Test";
	_input_json_name += ".json";
	Chapter::LoadSlideValue(_input_json_name.c_str(), *m_chapter);

	for (auto& _slide : m_chapter->GetSlides())
	{
		for (auto _character : _slide.m_characters)
		{
			std::string _key = _character->name;
			auto _facialSetItr = std::find_if(
				_allFacialSet.begin(), _allFacialSet.end(),
				[&](std::pair<std::string, std::shared_ptr<FacialSet>>& pair)
				{
					return pair.first == _key;
				}
			);
			_character->facialSet = _facialSetItr->second;
			//facialIndexに値を入れる(facialStrから値を取得)
			_character->facialIndex = _character->GetKeyIndex();

			for (auto& _charaAction : _character->m_actions)
			{
				_charaAction->Initialize();
			}
		}
		if (_slide.m_backSprFilePath.size())
		{
			_slide.m_backSpr = std::make_shared<Sprite>(ToWString(_slide.m_backSprFilePath).c_str());
		}

		strncpy_s(_slide.m_inputBuffer, sizeof(_slide.m_inputBuffer), _slide.m_text.c_str(), _TRUNCATE);
	}
#endif
}

void SceneTool::Finalize()
{
	for (Slide& _slide : m_chapter->GetSlides())
	{
		wchar_t _wideBuffer[256] = {};
		MultiByteToWideChar(CP_UTF8, 0, _slide.m_inputBuffer, -1, _wideBuffer, 256);
		_slide.m_text = ConvertWideToUtf8(_wideBuffer);
	}
	std::string _outputJsonName = "./Data/Json/";
	_outputJsonName += "/Chapter/";
	_outputJsonName += "Test";
	_outputJsonName += ".json";
	Chapter::SaveSlideValue(_outputJsonName.c_str(), *m_chapter);
}

void SceneTool::Update(float a_elapsedTime)
{
	Keyboard& _keyboard = Input::Instance().GeKeyboard();
	Graphics& _graphics = Graphics::Instance();
	Mouse& _mouse = Input::Instance().GetMouse();

	using SeparateType = ScreenSeparateLine::SeparateType;

	/*if (_keyboard.GetKeyInput(Keyboard::F3, Keyboard::DownMoment))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTool));
	}*/

	//モード切り替え
	ModeChange();

	//編集モードの時のみ
	if (m_mode == Mode::Edit)
	{
		//分割線用のパラメーター更新
		m_eventWindowRate = (1.0f - CharactersWindowRate) / 3.0f;
		m_charactersWindowWidth = screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) * CharactersWindowRate - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f;
		m_eventWindowWidth = (screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) - m_charactersWindowWidth) / 3.0f;
		m_eventWindowDrawStartPos.x = screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f + m_charactersWindowWidth;
		m_eventWindowDrawStartPos.y = screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)].y * 0.5f;
		m_slideWindowHeight = screenSize.y * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]);
	}

	//レビュー画面パラメーター更新
	ReviewBoardUpdate();
	//矩形UIパラメーター更新
	RectUIUpdate(a_elapsedTime);
	//スライドジャンプ用UIのパラメーター更新
	SlideJumpUpdate();

	//ギズモが操作されていなければ、矩形UIとマウスの当り判定チェック
	if (!m_usingGuizmo && !_graphics.IsWindowResizing())
	{
		//マウスと分割線の当り判定を行う
		m_screenSeparateLine.MouseHitCheck();
		//マウスと矩形UIとの当り判定を行う
		RectUIHitCheck();
		//マウスとスライドジャンプ用UIの当り判定
		SlideJumpHitCheck(a_elapsedTime);
	}

	//chapterのTool用のUpdateを呼ぶ
	switch (m_mode)
	{
	case SceneTool::Mode::Edit: //編集モード
		m_chapter->ToolUpdate(a_elapsedTime, m_reviewScreenLeftTopPos, m_reviewScreenSize);
		break;
	case SceneTool::Mode::Slideshow: //スライドショーモード
		if (!m_chapter->Update(a_elapsedTime))
		{
			m_chapter->EndSlideShow();
			m_mode = Mode::EndSlideShow;
		}
		break;
	}
}

void SceneTool::ReviewBoardUpdate()
{
	Graphics& _graphics = Graphics::Instance();
	using SeparateType = ScreenSeparateLine::SeparateType;

	//編集モードの時のみ
	if (m_mode == Mode::Edit)
	{
		m_screenSeparateLine.LineMove();//分割線を動かす
		m_reviewScreenAspectRate = m_reviewScreenNormalSize.y / m_reviewScreenNormalSize.x;//レビュー画面のアスペクト比を更新

		float _lineWidth = 5.0f;  //分割線の幅(画面の大きさが 1920：1080 の時を想定した値)
		//分割線の描画のためのパラメーター(Position,Size)の設定
		{
			m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)] = { screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)],0.0f };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)] = { _lineWidth * _graphics.GetFeelingSize(), screenSize.y };
			m_screenSeparateLine.linePosition[sc_i(SeparateType::LeftHorizontal)] = { 0.0f,screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)] = { screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
				_lineWidth * _graphics.GetFeelingSize() };
			m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)] = { screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + (_lineWidth / 2.0f * _graphics.GetFeelingSize()),
				screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)] = { screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]), _lineWidth * _graphics.GetFeelingSize() };
		}
	}

	//レビュー画面の描画のためのパラメーター(Position(中心),Size)の設定
	{
		m_reviewScreenPos.x = (screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) + (screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) / 2.0f);
		m_reviewScreenPos.y = (screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]) / 2.0f;
	}
	//横のサイズから決めて縦のサイズは画面のアスペクト比を用いて計算
	{
		//編集モードの時のみ
		if (m_mode == Mode::Edit)
		{
			m_reviewScreenSize.x = (screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)])) * m_reviewOffsetScale;
			m_reviewScreenSize.y = m_reviewScreenSize.x * m_reviewScreenAspectRate;
			m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			//もし、縦のサイズが大きすぎたら、
			if (m_reviewScreenSize.y > screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] * m_reviewOffsetScale)
			{
				//縦のサイズを補正してからそれに対応する横のサイズを画面のアスペクト比を用いて計算
				m_reviewScreenSize.y = screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] * m_reviewOffsetScale;
				m_reviewScreenSize.x = m_reviewScreenSize.y / m_reviewScreenAspectRate;
				m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
				m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			}
		}

		//レビュー画面を描画しているスペース全体のサイズを取得
		m_blackSpaceSize.x = screenSize.x - (m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)].x + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f);
		m_blackSpaceSize.y = (m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)].y - m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)].y * 0.5f);
		//レビュー画面を描画しているスペース全体からレビュー画面のサイズを引いた余白の大きさ取得
		m_remainingBlackSpaceSize.x = m_reviewScreenLeftTopPos.x - (m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)].x + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f);
		m_remainingBlackSpaceSize.y = m_reviewScreenLeftTopPos.y;
	}
}

void SceneTool::RectUIUpdate(float a_elapsedTime)
{
	using SeparateType = ScreenSeparateLine::SeparateType;

	float _minBlackSpace = m_remainingBlackSpaceSize.x <= m_remainingBlackSpaceSize.y ? m_remainingBlackSpaceSize.x : m_remainingBlackSpaceSize.y;

	//左右の矢印
	{
		//位置はreviewBoardの横に
		m_rectUIs["LTriangle"]->position = { m_reviewScreenLeftTopPos.x - m_remainingBlackSpaceSize.x / 2.0f ,m_reviewScreenPos.y };
		m_rectUIs["RTriangle"]->position = { m_reviewScreenRightBottomPos.x + m_remainingBlackSpaceSize.x / 2.0f,m_reviewScreenPos.y };
		//サイズは黒い余白の幅から算出
		m_rectUIs["LTriangle"]->size.x = m_rectUIs["RTriangle"]->size.x = m_remainingBlackSpaceSize.x * 0.8f;
		m_rectUIs["LTriangle"]->size.y = m_rectUIs["RTriangle"]->size.y = m_rectUIs["LTriangle"]->size.x * m_rectUIs["LTriangle"]->sprite->GetAspectRation();
		float _limitHeight = m_reviewScreenSize.y / 3.0f;
		if (m_rectUIs["LTriangle"]->size.y > _limitHeight)
		{
			m_rectUIs["LTriangle"]->size.y = m_rectUIs["RTriangle"]->size.y = _limitHeight;
			m_rectUIs["LTriangle"]->size.x = m_rectUIs["RTriangle"]->size.x = m_rectUIs["LTriangle"]->size.y / m_rectUIs["LTriangle"]->sprite->GetAspectRation();
		}
	}

	//デリートボタン
	{
		//位置は右下
		m_rectUIs["DustBox"]->position = { screenSize.x, m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)].y - m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)].y * 0.5f };
		m_rectUIs["DustBox"]->size.x = m_rectUIs["DustBox"]->size.y = _minBlackSpace;
	}
	//スライド追加ボタン
	{
		//位置は右上
		m_rectUIs["Add"]->position = { screenSize.x, 0.0f };
		//サイズはデリートボタンと同じに
		m_rectUIs["Add"]->size = m_rectUIs["DustBox"]->size;
	}

	for (const auto& [_, _rectUI] : m_rectUIs)
	{
		//矩形UIのパラメーター更新
		_rectUI->Update(a_elapsedTime);
	}
}

void SceneTool::RectUIHitCheck()
{
	Mouse& _mouse = Input::Instance().GetMouse();

	if (m_rectUIs["LTriangle"]->MouseHitCheck(_mouse)) //左矢印をダブルクリックしていたら
	{
		//1つ前のスライドへ移動(選択中のスライドが１枚目(０枚目)のスライドでなければ)
		if (m_chapter->m_currentSlideIndex > 0)
		{
			m_chapter->m_currentSlideIndex--;
		}
	}
	if (m_rectUIs["RTriangle"]->MouseHitCheck(_mouse)) //右矢印をダブルクリックしていたら
	{
		//1つ次のスライドへ移動
		m_chapter->m_currentSlideIndex++;
		//移動前のスライドがもし、最後のスライドだった場合
		if (m_chapter->GetSlides().size() == m_chapter->m_currentSlideIndex)
		{
			//スライド一枚追加
			m_chapter->GetSlides().emplace_back(Slide());
		}
	}
	if (m_chapter->GetSlides().size() >= 2) //スライドが１枚以下の時はデリートできないように
	{
		if (m_rectUIs["DustBox"]->MouseHitCheck(_mouse)) //ゴミ箱アイコンをダブルクリックしていたら
		{
			//選択中のスライドを削除
			m_chapter->GetSlides().erase(m_chapter->GetSlides().begin() + m_chapter->m_currentSlideIndex);
			if (m_chapter->m_currentSlideIndex >= m_chapter->GetSlides().size())
			{
				m_chapter->m_currentSlideIndex = m_chapter->GetSlides().size() - 1;
			}
		}
	}
	if (m_rectUIs["Add"]->MouseHitCheck(_mouse)) //＋(プラス)アイコンをダブルクリックしていたら
	{
		//スライド１枚追加
		m_chapter->GetSlides().insert(m_chapter->GetSlides().begin() + m_chapter->m_currentSlideIndex + 1, Slide());
		m_chapter->m_currentSlideIndex++;
	}

	//現在選択中のスライドを m_currentSlideIndex を基に更新
	m_chapter->m_currentSlide = &m_chapter->GetSlides()[m_chapter->m_currentSlideIndex];
}

void SceneTool::SlideJumpUpdate()
{
	//スライドジャンプ用UI全体のサイズ＆位置更新
	m_slideJumpUISize.x = m_reviewScreenSize.x * 0.9f;
	m_slideJumpUISize.y = m_remainingBlackSpaceSize.y * m_slideJumpUIHeightRate;
	if (m_slideJumpUISize.y > screenSize.y * 0.1f)
	{
		m_slideJumpUISize.y = screenSize.y * 0.1f; //高さ制限
	}
	//複数の表示するかジャンプ用UIの1つ目(最も左から)の描画位置
	slideJumpUIDrawStartPos = { m_reviewScreenPos.x - (m_slideJumpUISize.x / (m_chapter->GetSlides().size() * 2)) * (m_chapter->GetSlides().size() - 1), m_slideJumpUISize.y / 2.0f };
	//複数の表示するかジャンプ用UI１つ分の横幅を計算
	m_slideJumpUIDrawWidth = m_slideJumpUISize.x / m_chapter->GetSlides().size();
}

void SceneTool::SlideJumpHitCheck(float a_elapsedTime)
{
	Mouse& _mouse = Input::Instance().GetMouse();
	m_slideClickTimer -= a_elapsedTime;
	//左上、右下の座標取得
	XMFLOAT2 _leftTopPos = { m_reviewScreenPos.x - m_slideJumpUISize.x / 2.0f ,slideJumpUIDrawStartPos.y - m_slideJumpUISize.y / 2.0f };
	XMFLOAT2 _rightBottomPos = { m_reviewScreenPos.x + m_slideJumpUISize.x / 2.0f ,slideJumpUIDrawStartPos.y + m_slideJumpUISize.y / 2.0f };
	if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
		_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
	{
		m_touchSlideIndex = -1;
		return;
	}

	if (m_touchSlideIndex >= 0 && _mouse.GetButtonDown() & Mouse::BTN_LEFT)
	{
		if (m_slideClickTimer > 0.0f)
		{
			m_slideClickTimer = 0.0f;
			m_chapter->m_currentSlideIndex = m_touchSlideIndex;
			//現在選択中のスライド更新
			m_chapter->m_currentSlide = &m_chapter->GetSlides()[m_chapter->m_currentSlideIndex];
			return;
		}
		else
		{
			m_slideClickTimer = m_doubleClickTime;
		}
	}
	for (int i = 0; i < m_chapter->GetSlides().size(); i++)
	{
		_leftTopPos = { slideJumpUIDrawStartPos.x - m_slideJumpUIDrawWidth * slideJumpUIScaleRate / 2.0f + m_slideJumpUIDrawWidth * i ,
			slideJumpUIDrawStartPos.y - m_slideJumpUISize.y * slideJumpUIScaleRate / 2.0f };
		_rightBottomPos = { slideJumpUIDrawStartPos.x + m_slideJumpUIDrawWidth * slideJumpUIScaleRate / 2.0f + m_slideJumpUIDrawWidth * i,
			slideJumpUIDrawStartPos.y + m_slideJumpUISize.y * slideJumpUIScaleRate / 2.0f };
		if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
			_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
		{
			continue;
		}
		else
		{
			if (m_touchSlideIndex != i)
			{
				m_slideClickTimer = 0.0f;
			}
			m_touchSlideIndex = i;
			return;
		}
	}
	m_touchSlideIndex = -1;
}

void SceneTool::LineUpdate()
{
	if (m_lines.size())
	{
		int _i = 0;
		float _remainedSpace = 1.0f - lineNormalizeWallDistance;
		if (m_lines.size() == 1)
		{
			m_lines[0] = 0.5f;
		}
		else
		{
			float _lineToLineSpace = _remainedSpace / (m_lines.size() - 1);
			float _lineStartPosition = lineNormalizeWallDistance / 2.0f + (_remainedSpace * (1.0f - lineNormalizeDistance) / 2.0f);
			for (float& _linePos : m_lines)
			{
				if (_i == 0)
				{
					_linePos = _lineStartPosition;
				}
				else
				{
					_linePos = _lineStartPosition + (_lineToLineSpace * lineNormalizeDistance * _i);
				}
				_i++;
			}
		}
	}
}

void SceneTool::Render(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	RenderingState* _rendering_state = _graphics.GetRenderingState();
	Mouse& _mouse = Input::Instance().GetMouse();

	using SeparateType = ScreenSeparateLine::SeparateType;

	//RenderTargetのClear
	ClearRenderTarget();

	m_cb_fade->activate(_immediate_context, 3, CB_USAGE::P);

	m_fb_fade->Clear(_immediate_context, m_backColor.x, m_backColor.y, m_backColor.z, m_backColor.w);
	m_fb_fade->Activate(_immediate_context);

	//2Dスプライト描画
	{
		_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
		_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
		_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);


		switch (m_mode)
		{
		case SceneTool::Mode::Edit:
			m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize, 0.0f, m_reviewScreenColor);
			m_chapter->ToolRender(m_reviewScreenLeftTopPos, m_reviewScreenSize, m_mode != Mode::Edit ? true : false);
			break;
		case SceneTool::Mode::Slideshow:
			m_chapter->SlideshowRender();
			break;
		case SceneTool::Mode::EndSlideShow:
			m_chapter->EndSlideshowRender();
			break;
		default:
			break;
		}

		/*if (m_lines.size())
		{
			for (const float _line : m_lines)
			{
				float _posX = m_reviewScreenLeftTopPos.x + m_reviewScreenSize.x * _line;
				m_spriteBox[sc_i(SpriteKind::White)]->Render(BasePoint::Top, { _posX,m_reviewScreenLeftTopPos.y }, { m_reviewScreenSize.x / 300.0f,m_reviewScreenSize.y }, 0.0f, { 1.0f,0.0f,0.0f,1.0f });
			}
			m_spriteBox[sc_i(SpriteKind::White)]->Render(BasePoint::LeftTop, m_reviewScreenLeftTopPos, { m_reviewScreenSize.x * lineNormalizeWallDistance / 2.0f,m_reviewScreenSize.y }, 0.0f, { 0.0f,0.0f,0.0f,0.75f });
			m_spriteBox[sc_i(SpriteKind::White)]->Render(BasePoint::RightTop, { m_reviewScreenRightBottomPos.x,m_reviewScreenLeftTopPos.y }, { m_reviewScreenSize.x * lineNormalizeWallDistance / 2.0f,m_reviewScreenSize.y }, 0.0f, { 0.0f,0.0f,0.0f,0.75f });
		}*/

		//ページ数描画
		{
			if (m_chapter && m_chapter->GetSlides().size() && m_mode != Mode::EndSlideShow)
			{
				int _slideNum = m_chapter->m_currentSlideIndex + 1;//今のページ数
				int _tenRate = 1;//今のページ数が何桁かをこの変数に入れる
				for (int i = 1; i * 10 <= _slideNum; i *= 10)
				{
					_tenRate++;
				}
				XMFLOAT2 _slideNumPos;
				switch (m_mode)
				{
				case SceneTool::Mode::Slideshow:
					_slideNumPos = { 0.0f,0.0f };
					break;
				default:
					_slideNumPos = m_reviewScreenLeftTopPos;
					break;
				}
				XMFLOAT2 _slideNumSize = { m_reviewScreenSize.x / 25.0f,0.0f }; _slideNumSize.y = _slideNumSize.x;
				_slideNumSize.x /= _tenRate;//ページ数の1桁当たりの描画サイズを桁数に合わせて調整
				_slideNumPos.x += _slideNumSize.x * (_tenRate - 1);
				float offset_x = 0.0f;//2桁以上描画するときの数字描画位置
				for (int i = 1; i <= _slideNum; i *= 10)
				{
					XMFLOAT4 _color = { 1.0f,m_chapter->GetSlides().size() == _slideNum ? 0.3f : 1.0f,m_chapter->GetSlides().size() == _slideNum ? 0.3f : 1.0f, m_chapter->GetSlides().size() == _slideNum ? 0.8f : 0.5f };
					if (i == 1)
					{
						m_spriteBox.at(sc_i(SpriteKind::Number))->Render(BasePoint::LeftTop, _slideNumPos, _slideNumSize,
							static_cast<float>(((static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureWidth()) / 10.0f) * static_cast<float>(_slideNum % 10))), 0.0f, static_cast<float>((static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureWidth()) / 10.0f)), static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureHeight()), 0.0f, _color);
					}
					else
					{
						m_spriteBox.at(sc_i(SpriteKind::Number))->Render(BasePoint::LeftTop, _slideNumPos.x + offset_x, _slideNumPos.y, _slideNumSize,
							static_cast<float>(((static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureWidth()) / 10.0f) * static_cast<float>(_slideNum / i))), 0.0f, static_cast<float>((static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureWidth()) / 10.0f)), static_cast<float>(m_spriteBox.at(sc_i(SpriteKind::Number))->GetTextureHeight()), 0.0f, _color);
					}
					offset_x -= _slideNumSize.x;
				}
			}
		}
	}
	m_fb_fade->Deactivate(_immediate_context);

	_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
	_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
	_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);
	m_bitBlockTransfer->Blit(_immediate_context, m_fb_fade->shader_resource_views[0].GetAddressOf(), 0, 1, m_fb_ps_fade.Get());

	_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
	_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
	_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);
	//最大スケールでレビュー画面が表示されていないなら
	if (m_mode == Mode::Edit)
	{
		//画面を４分割する線を描画する
		{
			//VerticalLine
			m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Top, m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)],
				m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)], 0.0f, { 1.0f,1.0f,1.0f,m_screenSeparateLine.lineTouchFlag[sc_i(SeparateType::Vertical)] ? 1.0f : 0.3f });
			//LeftHorizontalLine
			m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Left, m_screenSeparateLine.linePosition[sc_i(SeparateType::LeftHorizontal)],
				m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)], 0.0f, { 1.0f,1.0f,1.0f,m_screenSeparateLine.lineTouchFlag[sc_i(SeparateType::LeftHorizontal)] ? 1.0f : 0.3f });
			//RightHorizontalLine
			m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Left, m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)],
				m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)], 0.0f, { 1.0f,1.0f,1.0f,m_screenSeparateLine.lineTouchFlag[sc_i(SeparateType::RightHorizontal)] ? 1.0f : 0.3f });
		}
		//選択スライド移動用の三角形描画
		{
			if (m_chapter->m_currentSlideIndex > 0)
			{
				m_rectUIs["LTriangle"]->Render({ 1.0f,1.0f,1.0f,m_rectUIs["LTriangle"]->mouseTouchFlag ? 1.0f : 0.5f });
			}
			int _slideNum = m_chapter->m_currentSlideIndex + 1;
			m_rectUIs["RTriangle"]->Render(DirectX::XMFLOAT2{ -1.0f,1.0f }, { 1.0f, 1.0f, m_chapter->GetSlides().size() == _slideNum ? 0.3f : 1.0f, m_rectUIs["RTriangle"]->mouseTouchFlag ? 1.0f : 0.5f });
		}
		//ゴミ箱アイコン描画
		{
			m_rectUIs["DustBox"]->Render({ 1.0f, 0.3f,0.3f, m_rectUIs["DustBox"]->mouseTouchFlag ? 1.0f : 0.5f });
		}
		//スライド追加アイコン描画
		{
			m_rectUIs["Add"]->Render({ 0.0f, 1.0f, 0.9f, m_rectUIs["Add"]->mouseTouchFlag ? 1.0f : 0.5f });
		}

		//スライド移動用UI描画
		m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Center, { m_reviewScreenPos.x , m_slideJumpUISize.y / 2.0f }, m_slideJumpUISize,
			0.0f, { 0.5f,0.5f,1.0f,0.5f });
		for (int i = 0; i < m_chapter->GetSlides().size(); i++)
		{
			if (m_touchSlideIndex == i)
			{
				m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Center, { slideJumpUIDrawStartPos.x + m_slideJumpUIDrawWidth * i,slideJumpUIDrawStartPos.y },
					m_slideJumpUIDrawWidth, m_slideJumpUISize.y, 0.0f, { 1.0f,1.0f,0.0f,1.0f });
			}
			m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Center, { slideJumpUIDrawStartPos.x + m_slideJumpUIDrawWidth * i,slideJumpUIDrawStartPos.y },
				m_slideJumpUIDrawWidth * slideJumpUIScaleRate, m_slideJumpUISize.y * slideJumpUIScaleRate, 0.0f, { i == m_chapter->m_currentSlideIndex ? 1.0f : 0.5f ,i == m_chapter->m_currentSlideIndex ? 1.0f : 0.5f,i == m_chapter->m_currentSlideIndex ? 1.0f : 0.5f,1.0f });
		}
	}
}

#ifdef USE_IMGUI
//ImGui描画関数
void SceneTool::ImGuiRender()
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	switch (m_mode)
	{
	case SceneTool::Mode::Edit:
		ImGuizmoRender();
		ImGuiAssetsWindow(ImGui::GetWindowSize().x * 0.85f);
		ImGuiSlideWindow();
		ImGuiOperationWindow();
		//Scene::CommonImGuiRender(_windowPos, _windowSize, _flags);
		break;
	case SceneTool::Mode::Slideshow:
		ImGuiSlideshowWindow();
		break;
	case SceneTool::Mode::EndSlideShow:
		ImGuiEndSlideshowWindow();
		break;
	default:
		break;
	}
}

void SceneTool::ImGuiOperationWindow()
{
	Graphics& _graphics = Graphics::Instance();
	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowPos = { 0.0f,screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	XMFLOAT2 _windowSize = { screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
		screenSize.y * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)]) - m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; //ウィンドウを動かせない＆閉じないようにする

	ImGui::SetNextWindowPos(ImVec2(_windowPos.x, _windowPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::Begin("Operation", nullptr, _flags);
	{
		if (m_chapter->m_currentSlide->m_characters.size())
		{
			if (ImGui::TreeNodeEx("Character", ImGuiTreeNodeFlags_DefaultOpen))
			{
				////操作キャラクターの変更(SliderIntによるIndex操作)
				//ImGui::SliderInt("Character Index", &m_chapter->m_currentSlide->m_characterIndex, 0, m_chapter->m_currentSlide->m_characters.size() - 1);

				//選択中のキャラクター表示
				{
					const auto _character = m_chapter->m_currentSlide->m_characters.at(m_chapter->m_currentSlide->m_characterIndex);
					Sprite* _characterSprite = _character->facialSet->GetFacial(_character->facialIndex).get();
					float _imageLimitSize = screenSize.y * 0.05f;
					ImVec2 _imageSize = { _imageLimitSize, 0.0f };
					_imageSize.y = _imageSize.x * _characterSprite->GetAspectRation();
					if (_imageSize.y > _imageLimitSize)
					{
						_imageSize.y = _imageLimitSize;
						_imageSize.x = _imageSize.y / _characterSprite->GetAspectRation();
					}
					ImGui::Image(_characterSprite->GetShaderResource(), _imageSize);
				}

				ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.5f, 0.2f, 0.5f)); //色変更
				ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.9f, 0.2f, 0.7f)); //色変更
				ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2f, 0.9f, 0.2f, 0.5f)); //色変更
				//レイヤーによる選択キャラクターの変更＆Zオーダーの操作
				if (ImGui::CollapsingHeader("Layer", ImGuiTreeNodeFlags_DefaultOpen))
				{
					for (int i = m_chapter->m_currentSlide->m_characters.size() - 1; i >= 0; i--)
					{
						ImGui::PushID(i);

						// グループで囲んで1行に画像＋名前を表示
						ImGui::BeginGroup();

						// Selectable 領域の始まり
						if (ImGui::Selectable(m_chapter->m_currentSlide->m_characters[i]->name.c_str()))
						{
							m_chapter->m_currentSlide->m_characterIndex = i;
						}
						ImGui::SameLine();

						auto& _character = m_chapter->m_currentSlide->m_characters.at(i);

						int _index = _character->facialIndex;
						Sprite* _facialSprite = _character->facialSet->GetFacial(_index).get();
						ImVec2 _imageSize = ImGui::GetItemRectSize();
						_imageSize.x = _imageSize.y / _facialSprite->GetAspectRation();
						ImGui::Image(_facialSprite->GetShaderResource(), _imageSize);

						ImGui::EndGroup();

						// ドラッグ元
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							ImGui::SetDragDropPayload("DND_ITEM", &i, sizeof(int));
							ImGui::Text("%s", m_chapter->m_currentSlide->m_characters[i]->name.c_str());
							ImGui::EndDragDropSource();
						}

						// ドロップ先
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ITEM"))
							{
								int payloadIndex = *(const int*)payload->Data;

								if (payloadIndex != i)
								{
									std::swap(m_chapter->m_currentSlide->m_characters[i], m_chapter->m_currentSlide->m_characters[payloadIndex]);

									if (m_chapter->m_currentSlide->m_characterIndex == payloadIndex)
										m_chapter->m_currentSlide->m_characterIndex = i;
									else if (m_chapter->m_currentSlide->m_characterIndex == i)
										m_chapter->m_currentSlide->m_characterIndex = payloadIndex;
								}
							}
							ImGui::EndDragDropTarget();
						}

						ImGui::PopID();
					}
					ImGui::Separator();
					ImGui::Separator();
					ImGui::Separator();
				}
				ImGui::PopStyleColor(3);	//色変更終了処理
				ImGui::TreePop();
			}
		}

		//ImGuizmoのモード変更
		if (ImGui::TreeNodeEx("Guozmo", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("Character", &m_guizmoOperateObject, sc_i(GuizmoOperateObject::Character)); ImGui::SameLine();
			ImGui::RadioButton("CharacterAction", &m_guizmoOperateObject, sc_i(GuizmoOperateObject::CharacterAction));
			if (m_guizmoOperateObject == sc_i(GuizmoOperateObject::CharacterAction) && m_chapter->m_currentSlide->m_characters.size())
			{
				auto& _character = m_chapter->m_currentSlide->m_characters[m_chapter->m_currentSlide->m_characterIndex];
				if (_character->m_actions.size())
				{
					if (selectActionIndex >= _character->m_actions.size())
					{
						selectActionIndex = _character->m_actions.size() - 1;
					}
					ImGui::SliderInt("selectAction", &selectActionIndex, 0, _character->m_actions.size() - 1);
					_character->m_actions[selectActionIndex]->OperateData();
				}
			}
			if (ImGui::TreeNodeEx("Guizmo Mode", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::RadioButton("TRANSLATE", &m_guizmoType, sc_i(ImGuizmo::OPERATION::TRANSLATE));
				ImGui::SameLine();
				ImGui::RadioButton("SCALE", &m_guizmoType, sc_i(ImGuizmo::OPERATION::SCALE));
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("SlideActions", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto& _action : m_chapter->m_currentSlide->m_actions)
			{
				_action->OperateData();
			}
			ImGui::TreePop();
		}

		/*if (ImGui::TreeNodeEx("Lines", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::Button("Add Line"))
			{
				m_lines.emplace_back();
			}
			if (m_lines.size())
			{
				if (ImGui::Button("Delete Line"))
				{
					m_lines.erase(m_lines.begin());
				}

				ImGui::InputFloat("Wall Distance", &lineNormalizeWallDistance, 0.01f);
				lineNormalizeWallDistance = std::clamp(lineNormalizeWallDistance, 0.0f, 1.0f);
				ImGui::InputFloat("Distance", &lineNormalizeDistance, 0.01f);
				lineNormalizeDistance = std::clamp(lineNormalizeDistance, 0.0f, 1.0f);
			}

			ImGui::TreePop();
		}
		ImGui::Separator();*/
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiSlideshowWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always); //ウィンドウの位置決定
	//ウィンドウの大きさ決定(分割線を基に計算)
	XMFLOAT2 _windowSize = { screenSize.x * 0.2f,screenSize.y * 0.4f};
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; //ウィンドウを動かせない＆閉じないようにする
	//カラーリング決定(灰色ベース)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); //非アクティブ時
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.6f, 0.95f, 1.0f)); //アクティブ時
	ImGui::Begin("Slideshow", nullptr, _flags);
	{
		int _mode = sc_i(m_mode);
		ImGui::InputInt("Mode", &_mode);

		int _slideState = sc_i(m_chapter->m_currentSlide->m_state);
		ImGui::InputInt("Slide State", &_slideState);
		ImGui::InputFloat("Main Timer", &m_chapter->m_currentSlide->m_mainTimer);
		ImGui::InputFloat("Text Read Timer", &m_chapter->m_currentSlide->m_textReadTimer);
		ImGui::InputFloat("Read End Timer", &m_chapter->m_currentSlide->m_readEndTimer);
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiEndSlideshowWindow()
{
}

//Assetウィンドウ用のImGui描画関数
void SceneTool::ImGuiAssetsWindow(float a_buttonWidth)
{
	Graphics& _graphics = Graphics::Instance();
	using SeparateType = ScreenSeparateLine::SeparateType;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always); //ウィンドウの位置決定
	//ウィンドウの大きさ決定(分割線を基に計算)
	XMFLOAT2 _windowSize = { screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
		screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse; //ウィンドウを動かせない＆閉じないようにする
	//カラーリング決定(灰色ベース)
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); //非アクティブ時
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.6f, 0.95f, 1.0f)); //アクティブ時
	ImGui::Begin("Asset", nullptr, _flags);
	{
		//カラーリング決定(赤色ベース)
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.6f, 0.3f, 0.3f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.8f, 0.4f, 0.4f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("TextWindow", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//テキストウィンドウ用ImGui描画位置
			ImGuiTextWindow(_windowSize.x * 0.85f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理

		//カラーリング決定(青色ベース)
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.3f, 0.6f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 1.0f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.4f, 0.8f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("All Character Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//全登場キャラクター用ImGui描画位置
			ImGuiAllCharactersWindow(_windowSize.x * 0.4f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理

		//カラーリング決定(緑色ベース)
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.6f, 0.3f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.9f, 0.25f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.8f, 0.4f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("Back Sprite Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiBackSpriteWindow(_windowSize.x * 0.4f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理

		//カラーリング決定(黄色ベース)
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.6f, 0.6f, 0.3f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.9f, 0.9f, 0.25f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.8f, 0.8f, 0.4f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiActionsWindow(_windowSize.x * 0.4f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiTextWindow(float a_buttonWidth)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext();

	using SeparateType = ScreenSeparateLine::SeparateType;

	if (m_chapter->m_signBoards.size())
	{
		ImVec2 _windowSize = ImGui::GetWindowSize();	//現在のImGuiのウィンドウのサイズ取得
		//レビュー描画用の変数(座標)
		for (int i = 0; i < m_chapter->m_signBoards.size(); i++)
		{
			if (ImGui::TreeNodeEx(("Board : " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				float _boardSpriteScaleX = 0.6f;
				XMFLOAT2 _imageDrawSize = { _windowSize.x * _boardSpriteScaleX ,_windowSize.x * _boardSpriteScaleX * m_chapter->m_signBoards.at(i)->GetBoardSprite()->GetAspectRation() };
				//もし、縦のサイズが大きすぎたら、
				if (_imageDrawSize.y > screenSize.y * 0.2f)
				{
					float _boardSpriteScaleY = 0.15f;
					//縦のサイズを補正して、それに応じた横のサイズを設定する
					_imageDrawSize.y = screenSize.y * _boardSpriteScaleY;
					_imageDrawSize.x = _imageDrawSize.y / m_chapter->m_signBoards.at(i)->GetBoardSprite()->GetAspectRation();
				}
				//レビュー描画
				ImGui::Image(m_chapter->m_signBoards.at(i)->GetBoardSprite()->GetShaderResource(), ImVec2(_imageDrawSize.x, _imageDrawSize.y));
				ImGui::Separator();

				//テキスト書き込み
				int _rowCount = 1;
				for (int i = 0; i < std::strlen(m_chapter->m_currentSlide->m_inputBuffer); ++i)
				{
					if (m_chapter->m_currentSlide->m_inputBuffer[i] == '\n')
					{
						_rowCount++;
					}
				}

				ImGui::Text("Text");
				ImGui::InputTextMultiline(("##" + std::to_string(i)).c_str(),  // ラベル
					m_chapter->m_currentSlide->m_inputBuffer,           // テキストバッファ
					IM_ARRAYSIZE(m_chapter->m_currentSlide->m_inputBuffer), // バッファサイズ
					ImVec2(ImGui::GetWindowSize().x * 0.75f, screenSize.y * (0.025f * _rowCount)));

				ImGui::TreePop();
			}
		}

		int _textReadSpeed = sc_i(Options::GetInstance()->GetTextReadSpeed());
		ImGui::SliderInt("textReadSpeed", &_textReadSpeed, 0, sc_i(Options::TextReadSpeed::Max) - 1);
		Options::GetInstance()->SetTextReadSpeed(_textReadSpeed);

		ImGui::Separator();
	}

	if (ImGui::Button("Select TextWindow Json File", ImVec2(a_buttonWidth * 0.5f, 0.0f)))
	{
		static const char* filter = "Model Files(*.json;)\0*.json;\0All Files(*.*)\0*.*;\0\0";

		char _selectFileName[256] = { 0 };
		HWND _hWnd = Graphics::Instance().GetHWND();
		DialogResult _result = Dialog::OpenFileName(_selectFileName, sizeof(_selectFileName), filter, nullptr, _hWnd);
		if (_result == DialogResult::OK)
		{
			wchar_t _wideBuffer[256]; ////選択したファイルのパスを保存する(wchar_t <--- char)
			MultiByteToWideChar(CP_ACP, 0, _selectFileName, -1, _wideBuffer, 256); //(wchar_t <--- char)
			//絶対パスを相対パス化
			auto _jsonFilePath = std::filesystem::relative(_wideBuffer);
			m_chapter->m_signBoards.emplace_back(std::make_unique<SignBoard>(_device, _deviceContext, _jsonFilePath.string()));
		}
	}
	//m_textWindowがnullptrじゃなかったら
	if (m_chapter->m_signBoards.size())
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
		//Resetボタン(押すとtextWindowポインタをnullptrにする)
		if (ImGui::Button("Reset Text Window", ImVec2(a_buttonWidth * 0.5f, 0.0f)))
		{
			m_chapter->m_signBoards.clear();
		}
		ImGui::PopStyleColor(2);
	}
}

void SceneTool::ImGuiAllCharactersWindow(float a_buttonWidth)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext();
	auto _allFacIns = AllFacialSet::GetInstance();
	auto _allFacialSet = _allFacIns->GetAllFacialSet();

	std::vector<std::string> _characterKeys{};
	std::vector<const char*> _cstrItems;
	static int _currentItem = 0;
	float _imageLimitHeight = screenSize.y * 0.1f;
	if (_allFacialSet.size())
	{
		for (auto& [_key, _character] : _allFacialSet)
		{
			_characterKeys.push_back(_key);
		}
		// 一時的に const char* の配列を作成（ImGui::Combo 用）
		for (const auto& str : _characterKeys)
		{
			_cstrItems.push_back(str.c_str());
		}
		ImGui::Combo("Character", &_currentItem, _cstrItems.data(), static_cast<int>(_cstrItems.size()));

		Sprite* _characterSprite = _allFacIns->GetFacialSet(_currentItem)->GetFacial(0).get();
		ImVec2 _imageSize = { a_buttonWidth, a_buttonWidth * _characterSprite->GetAspectRation() };
		if (_imageSize.y > _imageLimitHeight)
		{
			_imageSize.y = _imageLimitHeight;
			_imageSize.x = _imageSize.y / _characterSprite->GetAspectRation();
		}
		ImGui::Image(_characterSprite->GetShaderResource(), _imageSize);
	}

	if (ImGui::Button("Add Select Character", ImVec2(a_buttonWidth, 0.0f)))
	{
		const std::string& _key = _characterKeys[_currentItem];
		auto _facialSetItr = std::find_if(
			_allFacialSet.begin(), _allFacialSet.end(),
			[&](std::pair<std::string, std::shared_ptr<FacialSet>>& pair)
			{
				return pair.first == _key;
			}
		);

		auto& appearingCharacters = m_chapter->m_currentSlide->m_characters;
		if (std::none_of(appearingCharacters.begin(), appearingCharacters.end(),
			[&](const auto& c) { return c->name == _facialSetItr->first; }))
		{
			appearingCharacters.emplace_back(std::make_shared<Character>(_facialSetItr->first, _facialSetItr->second));
		}
	}
}

void SceneTool::ImGuiBackSpriteWindow(float a_buttonWidth)
{
	if (ImGui::Button("Select BackSprite", ImVec2(a_buttonWidth, 0.0f)))
	{
		//filter(選択できるファイル形式)設定
		static const char* _filter = "Image Files (*.png;*.jpeg;*.jpg)\0*.png;*.jpeg;*.jpg\0""All Files (*.*)\0*.*\0\0";
		char _selectImageName[256] = {}; //選択したファイルのパスを保存する
		HWND _hWnd = Graphics::Instance().GetHWND();
		//開くファイルを選択する(選択し終えたら、_result = DialogResult::OK になる)
		DialogResult _result = Dialog::OpenFileName(_selectImageName, sizeof(_selectImageName), _filter, "開きたい.png または.jpegファイルを選択してください", _hWnd);
		//ファイルが選択されていたら
		if (_result == DialogResult::OK)
		{
			wchar_t wideBuffer[256]; ////選択したファイルのパスを保存する(wchar_t <--- char)
			MultiByteToWideChar(CP_ACP, 0, _selectImageName, -1, wideBuffer, 256); //(wchar_t <--- char)
			//絶対パスを相対パス化
			auto _spriteFilePath = std::filesystem::relative(wideBuffer);
			m_chapter->m_currentSlide->m_backSpr = std::make_shared<Sprite>(_spriteFilePath.wstring().c_str());
			m_chapter->m_currentSlide->m_backSprFilePath = WideToUtf8(_spriteFilePath.wstring());
		}
	}
	if (m_chapter->m_currentSlideIndex > 0 && m_chapter->GetSlides()[m_chapter->m_currentSlideIndex - 1].m_backSpr)
	{
		ImGui::SameLine();
		if (ImGui::Button("Copy Previous Slides BackSprite"))
		{
			m_chapter->m_currentSlide->m_backSpr = m_chapter->GetSlides()[m_chapter->m_currentSlideIndex - 1].m_backSpr;
		}
	}
	ImGui::Separator();
}

void SceneTool::ImGuiActionsWindow(float a_buttonWidth)
{
	std::vector<const char*> _cStrItems;
	static int _currentItem = 0;

	// 一時的に const char* の配列を作成（ImGui::Combo 用）
	for (const auto& str : slideActionStrList)
	{
		_cStrItems.push_back(str.c_str());
	}
	ImGui::Combo("SlidesAction", &_currentItem, _cStrItems.data(), static_cast<int>(_cStrItems.size()));

	if (ImGui::Button("Add Action", ImVec2(a_buttonWidth, 0.0f)))
	{
		m_chapter->m_currentSlide->AddAction(_currentItem);
	}
}

void SceneTool::ImGuiSlideWindow()
{
	ImGuiCharactersWindow();
	ImGuiEnterWindow();
	ImGuiExecuteWindow();
	ImGuiExitWindow();
}

void SceneTool::ImGuiCharactersWindow()
{
	Graphics& _graphics = Graphics::Instance();
	//画面サイズ
	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_charactersWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.6f, 0.15f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.3f, 1.0f, 1.0f));
	ImGui::Begin("Characters", nullptr, _flags);
	{
		if (m_chapter)
		{
			if (m_chapter->m_currentSlide->m_characters.size())
			{
				std::vector<std::string> _facialsKeys{};
				std::vector<const char*> _cstrItems;
				float _spriteSizeX = ImGui::GetWindowSize().x * 0.5f;
				XMFLOAT2 _imageLimitSize = { screenSize.x * 0.1f, screenSize.y * 0.1f };
				for (int i = m_chapter->m_currentSlide->m_characters.size() - 1; i >= 0; i--)
				{
					auto& _character = m_chapter->m_currentSlide->m_characters.at(i);

					//表情差分
					{
						_facialsKeys.clear();
						_cstrItems.clear();
						for (auto& _key : _character->facialSet->keys)
						{
							_facialsKeys.emplace_back(_key);	// ← ここで _facialsKeys が文字列の実体を保持
						}
						for (const auto& str : _facialsKeys)
						{
							_cstrItems.push_back(str.c_str());	// ← 安全にポインタを取得
						}

						std::string _comboLabel = "Facial##" + std::to_string(i);
						ImGui::Combo(_comboLabel.c_str(), &_character->facialIndex, _cstrItems.data(), static_cast<int>(_cstrItems.size()));
						_character->facialStr = _character->facialSet->GetKey(_character->facialIndex);

						int _index = _character->facialIndex;
						Sprite* _facialSprite = _character->facialSet->GetFacial(_index).get();
						ImVec2 _imageSize = { _imageLimitSize.x, _imageLimitSize.x * _facialSprite->GetAspectRation() };
						if (_imageSize.y > _imageLimitSize.y)
						{
							_imageSize.y = _imageLimitSize.y;
							_imageSize.x = _imageSize.y / _facialSprite->GetAspectRation();
						}
						ImGui::Image(_facialSprite->GetShaderResource(), _imageSize);
					}

					//アクション
					{
						std::vector<const char*> _cStrItems;
						static int _currentItem = 0;

						// 一時的に const char* の配列を作成（ImGui::Combo 用）
						for (const auto& str : characterActionStrList)
						{
							_cStrItems.push_back(str.c_str());
						}
						ImGui::Combo("CharctersAction", &_currentItem, _cStrItems.data(), static_cast<int>(_cStrItems.size()));
						if (ImGui::Button("Add Action"))
						{
							_character->AddAction(_currentItem);
						}
					}

					std::string _deleteButtonLabel = "Delete Character##" + std::to_string(i);
					if (ImGui::Button(_deleteButtonLabel.c_str(), ImVec2(_spriteSizeX, 0.0f)))
					{
						m_chapter->m_currentSlide->m_removes.insert(_character);
					}
					ImGui::Separator();
				}
			}
		}
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiEnterWindow()
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.5f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.3f, 0.3f, 1.0f));
	ImGui::Begin("SlideActions Enter Window", nullptr, _flags);
	{
		for (auto& _action : m_chapter->m_currentSlide->m_actions)
		{
			_action->ShowEnter();
		}
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiExecuteWindow()
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x + m_eventWindowWidth, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.15f, 0.5f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
	ImGui::Begin("SlideActions Execute Window", nullptr, _flags);
	{
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiExitWindow()
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x + m_eventWindowWidth * 2.0f, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.15f, 0.15f, 0.5f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.3f, 0.7f, 1.0f));
	ImGui::Begin("SlideActions Exit Window", nullptr, _flags);
	{
		for (auto& _action : m_chapter->m_currentSlide->m_actions)
		{
			_action->ShowExit();
		}
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuizmoRender()
{
	Graphics& _graphics = Graphics::Instance();

	ImGuizmo::SetOrthographic(true);
	ImVec2 _guizmoTaskPos = { m_reviewScreenLeftTopPos.x,m_reviewScreenLeftTopPos.y };
	ImVec2 _guizmoTaskSize = { m_reviewScreenSize.x,m_reviewScreenSize.y };
	ImGuizmo::SetRect(_guizmoTaskPos.x, _guizmoTaskPos.y, _guizmoTaskSize.x, _guizmoTaskSize.y);

	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)m_guizmoViewMat, XMMatrixIdentity());
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)m_guizmoProjMat, XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f));

	switch (static_cast<GuizmoOperateObject>(m_guizmoOperateObject))
	{
	case SceneTool::GuizmoOperateObject::Character:
		CharacterGuizmo();
		break;
	case SceneTool::GuizmoOperateObject::CharacterAction:
		CharacterActionsGuizmo();
		break;
	}
}
void SceneTool::CharacterGuizmo()
{
	if (!m_chapter->m_currentSlide->m_characters.size())return;

	//Guizmoの作業領域
	int _selectCharacterIndex = m_chapter->m_currentSlide->m_characterIndex;
	XMFLOAT4X4 _guizmoTransform;
	DirectX::XMMATRIX _selectS{}, _selectR{}, _selectT{};
	_selectR = DirectX::XMMatrixRotationZ(0.0f);
	_selectS = DirectX::XMMatrixScaling(m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.x, m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.y, 1.0f);
	_selectT = DirectX::XMMatrixTranslation(m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.x, m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.y, 0.0f);

	DirectX::XMStoreFloat4x4(&_guizmoTransform, _selectS * _selectR * _selectT);

	//行列用の変数
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	float view_mat[16], proj_mat[16];
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)view_mat, XMMatrixIdentity());
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)proj_mat, XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f));

	ImGuizmo::Manipulate(m_guizmoViewMat, m_guizmoProjMat, static_cast<ImGuizmo::OPERATION>(m_guizmoType), ImGuizmo::LOCAL, &_guizmoTransform._11);
	if (ImGuizmo::IsUsing())
	{
		XMVECTOR _scale, _rotation, _translation;
		XMMATRIX _newTransform = XMLoadFloat4x4(&_guizmoTransform);
		XMMatrixDecompose(&_scale, &_rotation, &_translation, _newTransform);

		XMStoreFloat2(&m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize, _scale);
		m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.x = std::clamp(
			m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.x, FLT_EPSILON, 5.0f);
		m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.y = std::clamp(
			m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizeSize.y, FLT_EPSILON, 5.0f);

		XMStoreFloat2(&m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition, _translation);
		m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.x = std::clamp(
			m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.x, FLT_EPSILON, 1.0f);
		m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.y = std::clamp(
			m_chapter->m_currentSlide->m_characters[_selectCharacterIndex]->normalizePosition.y, FLT_EPSILON, 1.0f);
	}
}

void SceneTool::CharacterActionsGuizmo()
{
	if (!m_chapter->m_currentSlide->m_characters.size())return;

	//Guizmoの作業領域
	auto& _character = m_chapter->m_currentSlide->m_characters[m_chapter->m_currentSlide->m_characterIndex];
	if (!_character->m_actions.size()) return;

	auto& _selectAction = _character->m_actions[selectActionIndex];
	if (!_selectAction->useImGui) return;

	XMFLOAT4X4 _guizmoTransform;
	DirectX::XMMATRIX _selectS{}, _selectR{}, _selectT{};

	_selectS = _selectAction->GuizmoScale();
	_selectR = _selectAction->GuizmoRotate();
	_selectT = _selectAction->GuizmoTranslate();
	DirectX::XMStoreFloat4x4(&_guizmoTransform, _selectS * _selectR * _selectT);

	//行列用の変数
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)m_guizmoViewMat, view);
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)m_guizmoProjMat, proj);

	ImGuizmo::Manipulate(m_guizmoViewMat, m_guizmoProjMat, static_cast<ImGuizmo::OPERATION>(m_guizmoType), ImGuizmo::LOCAL, &_guizmoTransform._11);
	if (ImGuizmo::IsUsing())
	{
		XMMATRIX _newTransform = XMLoadFloat4x4(&_guizmoTransform);
		_selectAction->GuizmoDecompose(_newTransform);
	}
}

#endif // USE_IMGUI

void SceneTool::OnSizeChange()
{
	for (auto& _flag : m_screenSeparateLine.lineHoldFlag)
	{
		_flag = false;
	}
	for (auto& _flag : m_screenSeparateLine.lineTouchFlag)
	{
		_flag = false;
	}
}

void SceneTool::ModeChange()
{
	Keyboard& _keyboard = Input::Instance().GeKeyboard();
	Graphics& _graphics = Graphics::Instance();
	Mouse& _mouse = Input::Instance().GetMouse();

	using SeparateType = ScreenSeparateLine::SeparateType;

	switch (m_mode)
	{
	case SceneTool::Mode::Edit:
		//F5を押すと最大スケールでレビュー画面で表示される
		if (_keyboard.GetKeyInput(Keyboard::F5, Keyboard::DownMoment))
		{
			m_mode = Mode::Slideshow;
			m_chapter->StartSlideshow();
			//CTRLキーが押されていたら、最初のスライドから再生
			if (_keyboard.GetKeyInput(Keyboard::CTRL, Keyboard::Down))
			{

			}
			//押されていなかったら、現在のスライドから再生
			else
			{
				m_chapter->m_currentSlideIndex = 0;
				m_chapter->m_currentSlide = &m_chapter->GetSlides()[m_chapter->m_currentSlideIndex];
				m_chapter->m_currentSlide->ActionsEnter();
			}

		}
		////F2を押すと最大スケールでレビュー画面で表示される
		//else if (_keyboard.GetKeyInput(Keyboard::F2, Keyboard::DownMoment))
		//{
		//	m_mode = Mode::Check;
		//	//最大表示前の分割パラメーターを保存しておく
		//	for (int i = 0; i < std::size(m_screenSeparateLine.beforeRate); i++)
		//	{
		//		m_screenSeparateLine.beforeRate[i] = m_screenSeparateLine.rate[i];
		//	}
		//	m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] = 0.0f;
		//	m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] = 1.0f;
		//}
		break;
		//case SceneTool::Mode::Check:
		//	if (_keyboard.GetKeyInput(Keyboard::ESC, Keyboard::DownMoment) || _keyboard.GetKeyInput(Keyboard::F2, Keyboard::DownMoment))
		//	{
		//		m_mode = Mode::Edit;
		//		//保存していたパラメーターに戻す
		//		for (int i = 0; i < std::size(m_screenSeparateLine.beforeRate); i++)
		//		{
		//			m_screenSeparateLine.rate[i] = m_screenSeparateLine.beforeRate[i];
		//		}
		//	}
		//	break;
	case SceneTool::Mode::Slideshow:
		if (_keyboard.GetKeyInput(Keyboard::ESC, Keyboard::DownMoment) || _keyboard.GetKeyInput(Keyboard::F5, Keyboard::DownMoment))
		{
			m_chapter->EndSlideShow();
			m_mode = Mode::Edit;
		}
		break;
	case SceneTool::Mode::EndSlideShow:
		if (_mouse.GetButtonDown() & Mouse::BTN_LEFT || _keyboard.GetKeyInput(Keyboard::ESC, Keyboard::DownMoment) || _keyboard.GetKeyInput(Keyboard::F5, Keyboard::DownMoment))
		{
			m_chapter->EndSlideShow();
			m_mode = Mode::Edit;
		}
		break;
	default:
		if (_keyboard.GetKeyInput(Keyboard::ESC, Keyboard::DownMoment))
		{
			m_mode = Mode::Edit;
		}
		break;
	}

}

void SceneTool::ScreenSeparateLine::MouseHitCheck()
{
	Mouse& _mouse = Input::Instance().GetMouse();

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _leftTopPos;
	XMFLOAT2 _rightBottomPos;

	auto _TouchAndHoldCheck = [&](int a_type, BasePoint a_basePoint)
		{
			_leftTopPos = CalcSquareLeftTopPosition(a_basePoint, linePosition[a_type], lineSize[a_type]);
			_rightBottomPos = CalcSquareRightBottomPosition(a_basePoint, linePosition[a_type], lineSize[a_type]);
			if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
				_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
			{
				lineTouchFlag[a_type] = false;
			}
			else
			{
				lineTouchFlag[a_type] = true;
			}

			if (lineTouchFlag[a_type])
			{
				if (_mouse.GetButtonDown() & Mouse::BTN_LEFT)
				{
					lineHoldFlag[a_type] = true;
				}
			}
			if (!(_mouse.GetButton() & Mouse::BTN_LEFT))
			{
				lineHoldFlag[a_type] = false;
			}
		};

	if (!lineHoldFlag[sc_i(SeparateType::LeftHorizontal)] && !lineHoldFlag[sc_i(SeparateType::RightHorizontal)])
	{
		_TouchAndHoldCheck(sc_i(SeparateType::Vertical), BasePoint::Top);
	}

	if (!lineHoldFlag[sc_i(SeparateType::Vertical)] && !lineHoldFlag[sc_i(SeparateType::RightHorizontal)])
	{
		_TouchAndHoldCheck(sc_i(SeparateType::LeftHorizontal), BasePoint::Left);
	}

	if (!lineHoldFlag[sc_i(SeparateType::LeftHorizontal)] && !lineHoldFlag[sc_i(SeparateType::Vertical)])
	{
		_TouchAndHoldCheck(sc_i(SeparateType::RightHorizontal), BasePoint::Left);
	}
}

void SceneTool::ScreenSeparateLine::LineMove()
{
	Graphics& _graphics = Graphics::Instance();
	XMFLOAT2 _screenSize = Scene::screenSize;
	Mouse& _mouse = Input::Instance().GetMouse();

	if (lineHoldFlag[sc_i(SeparateType::Vertical)])
	{
		if (_screenSize.x >= FLT_EPSILON)
		{
			rate[sc_i(SeparateType::Vertical)] = _mouse.GetPositionX() / _screenSize.x;
		}
		rate[sc_i(SeparateType::Vertical)] = std::clamp(rate[sc_i(SeparateType::Vertical)], 0.15f, 0.75f);
	}
	else if (lineHoldFlag[sc_i(SeparateType::LeftHorizontal)])
	{
		if (_screenSize.y >= FLT_EPSILON)
		{
			rate[sc_i(SeparateType::LeftHorizontal)] = _mouse.GetPositionY() / _screenSize.y;
		}
		rate[sc_i(SeparateType::LeftHorizontal)] = std::clamp(rate[sc_i(SeparateType::LeftHorizontal)], 0.025f, 0.975f);
	}
	else if (lineHoldFlag[sc_i(SeparateType::RightHorizontal)])
	{
		if (_screenSize.y >= FLT_EPSILON)
		{
			rate[sc_i(SeparateType::RightHorizontal)] = _mouse.GetPositionY() / _screenSize.y;
		}
		rate[sc_i(SeparateType::RightHorizontal)] = std::clamp(rate[sc_i(SeparateType::RightHorizontal)], 0.3f, 0.975f);
	}
}