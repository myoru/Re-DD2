#include <filesystem>
#include "Graphics.h"
#include "ImGuiPack.h"
#ifdef USE_IMGUI
#include "../External/imgui/ImGuizmo.h"
#endif
#include "SceneTool.h"
#include "Dialog.h"
#include "Input.h"
#include "AllFacialSet.h"

using namespace DirectX;

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

std::string ConvertWideToUtf8(const wchar_t* wideStr)
{
	if (!wideStr) return {};

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	if (sizeNeeded <= 0) return {};

	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], sizeNeeded, nullptr, nullptr);

	// erase null terminator
	if (!result.empty() && result.back() == '\0')
	{
		result.pop_back();
	}

	return result;
}

void SceneTool::Initialize()
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext();

	using SeparateType = ScreenSeparateLine::SeparateType;

	//Scene識別インデックス設定
	Scene::SetIndex(sc_i(Scene::Index::Event));

	////定数バッファ
	m_cb_fade->data.Initialize(5.0f, 1.0f, 1, 1, 0);

	m_spriteBox.emplace_back(std::make_unique<Sprite>(nullptr, BasePoint::LeftTop));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\SlideTriangle.png"));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\Number.png"));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\DustBox.png"));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\Add.png"));

	m_reviewScreenPos = { (_graphics.GetScreenWidth() * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) + (_graphics.GetScreenWidth() * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) / 2.0f),
		(_graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]) / 2.0f };
	m_reviewScreenAspectRate = m_reviewScreenNormalSize.y / m_reviewScreenNormalSize.x;
	m_reviewScreenSize.x = (_graphics.GetScreenWidth() * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)])) * 0.9f;
	m_reviewScreenSize.y = m_reviewScreenSize.x * m_reviewScreenAspectRate;

	m_chapter = std::make_unique<Chapter>();

	//SAMMARY
}

void SceneTool::Finalize()
{
	//TODO : 
	//TEst
}

void SceneTool::Update(float a_elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };
	Mouse& _mouse = Input::Instance().GetMouse();
	Keyboard& _keyboard = Input::Instance().GeKeyboard();

	using SeparateType = ScreenSeparateLine::SeparateType;
	using TriangleType = SlideTriangle::TriangleType;

	//F2を押すと最大スケールでレビュー画面で表示される
	if (_keyboard.GetKeyInput(Keyboard::F2, Keyboard::DownMoment))
	{
		m_reviewFullScreenTestFlag = !m_reviewFullScreenTestFlag;
		if (m_reviewFullScreenTestFlag)
		{
			//最大表示前の分割パラメーターを保存しておく
			for (int i = 0; i < std::size(m_screenSeparateLine.beforeRate); i++)
			{
				m_screenSeparateLine.beforeRate[i] = m_screenSeparateLine.rate[i];
			}
			m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] = 0.0f;
			m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] = 1.0f;
		}
		else
		{
			//保存していたパラメーターに戻す
			for (int i = 0; i < std::size(m_screenSeparateLine.beforeRate); i++)
			{
				m_screenSeparateLine.rate[i] = m_screenSeparateLine.beforeRate[i];
			}
		}
	}

	//最大スケールでレビュー画面が表示されていないなら
	if (!m_reviewFullScreenTestFlag)
	{
		m_screenSeparateLine.LineMove();//分割線を動かす
		m_reviewScreenAspectRate = m_reviewScreenNormalSize.y / m_reviewScreenNormalSize.x;//レビュー画面のアスペクト比を更新

		float _lineWidth = 5.0f;  //分割線の幅(画面の大きさが 1920：1080 の時を想定した値)
		//分割線の描画のためのパラメーター(Position,Size)の設定
		{
			m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)] = { _screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)],0.0f };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)] = { _lineWidth * _graphics.GetFeelingSize(), _screenSize.y };
			m_screenSeparateLine.linePosition[sc_i(SeparateType::LeftHorizontal)] = { 0.0f,_screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)] = { _screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
				_lineWidth * _graphics.GetFeelingSize() };
			m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)] = { _screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + (_lineWidth / 2.0f * _graphics.GetFeelingSize()),
				_screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] };
			m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)] = { _screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]), _lineWidth * _graphics.GetFeelingSize() };
		}
	}

	//レビュー画面の描画のためのパラメーター(Position(中心),Size)の設定
	{
		m_reviewScreenPos.x = (_graphics.GetScreenWidth() * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) + (_graphics.GetScreenWidth() * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) / 2.0f);
		m_reviewScreenPos.y = (_graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]) / 2.0f;
	}
	//横のサイズから決めて縦のサイズは画面のアスペクト比を用いて計算
	{
		//最大スケールでレビュー画面が表示されていないなら
		if (!m_reviewFullScreenTestFlag)
		{
			m_reviewScreenSize.x = (_graphics.GetScreenWidth() * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)])) * m_reviewOffsetScale;
			m_reviewScreenSize.y = m_reviewScreenSize.x * m_reviewScreenAspectRate;
			m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			//もし、縦のサイズが大きすぎたら、
			if (m_reviewScreenLeftTopPos.y < _graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] * 0.05f)
			{
				//縦のサイズを補正してからそれに対応する横のサイズを画面のアスペクト比を用いて計算
				m_reviewScreenSize.y = _graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] * m_reviewOffsetScale;
				m_reviewScreenSize.x = m_reviewScreenSize.y / m_reviewScreenAspectRate;
				m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
				m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			}
		}
		else
		{
			m_reviewScreenSize.x = _graphics.GetScreenWidth();
			m_reviewScreenSize.y = m_reviewScreenSize.x * m_reviewScreenAspectRate;
			m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			//もし、縦のサイズが大きすぎたら、
			if (m_reviewScreenLeftTopPos.y < 0.0f)
			{
				//縦のサイズを補正してからそれに対応する横のサイズを画面のアスペクト比を用いて計算
				m_reviewScreenSize.y = _graphics.GetScreenHeight();
				m_reviewScreenSize.x = m_reviewScreenSize.y / m_reviewScreenAspectRate;
				m_reviewScreenLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
				m_reviewScreenRightBottomPos = CalcSquareRightBottomPosition(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize);
			}
		}

		m_slideTriangle.scaleX = (1.0f - m_reviewOffsetScale) * 0.4f;

		m_slideTriangle.offsetX = m_reviewScreenLeftTopPos.x - (m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)].x + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f);
		m_slideTriangle.position[sc_i(TriangleType::Left)] = { m_screenSeparateLine.linePosition[sc_i(SeparateType::Vertical)].x + (m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f) + (m_slideTriangle.offsetX * 0.5f) ,m_reviewScreenPos.y };
		m_slideTriangle.position[sc_i(TriangleType::Right)] = { m_reviewScreenRightBottomPos.x + m_slideTriangle.offsetX * 0.5f,m_reviewScreenPos.y };
		m_slideTriangle.size = { m_slideTriangle.scaleX * (_screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)])) ,0.0f }; m_slideTriangle.size.y = m_slideTriangle.size.x * m_spriteBox.at(sc_i(SpriteKind::Triangle))->GetAspectRation();

		m_dustBoxPos = { _screenSize.x, m_screenSeparateLine.linePosition[sc_i(SeparateType::RightHorizontal)].y - m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)].y * 0.5f };
		m_dustBoxSize.x = m_dustBoxSize.y = m_slideTriangle.size.x;
		m_addIconPos = { _screenSize.x, 0.0f };
		m_addIconSize = m_dustBoxSize;
	}

	//看板(m_signBoard)が一つでも登録されていれば
	if (m_signBoards.size())
	{
		//更新処理を行う
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			m_signBoards.at(i)->Update(a_elapsedTime, m_reviewScreenLeftTopPos, m_reviewScreenSize);
		}
	}

	CharactersUpdate(a_elapsedTime);

	//ギズモが操作されていなければ
	if (!m_usingGuizmo)
	{
		//マウスと分割線の当り判定を行う
		m_screenSeparateLine.MouseHitCheck();
		MouseVsTriangles();
		MouseVsDustBox();
		MouseVsAddIcon();
	}

	m_eventWindowRate = (1.0f - m_charactersWindowRate) / 3.0f;
	m_charactersWindowWidth = _screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) * m_charactersWindowRate - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f;
	m_eventWindowWidth = (_screenSize.x * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)]) - m_charactersWindowWidth) / 3.0f;
	m_eventWindowDrawStartPos.x = _screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f + m_charactersWindowWidth;
	m_eventWindowDrawStartPos.y = _screenSize.y * m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::RightHorizontal)].y * 0.5f;
	m_slideWindowHeight = _screenSize.y * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::RightHorizontal)]);
}

void SceneTool::CharactersUpdate(float a_elapsedTime)
{
	if (m_chapter && m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().size())
	{
		m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->ToolUpdate(a_elapsedTime, m_reviewScreenLeftTopPos, m_reviewScreenSize);
	}
}

void SceneTool::Render(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	RenderingState* _rendering_state = _graphics.GetRenderingState();
	Mouse& _mouse = Input::Instance().GetMouse();

	using SeparateType = ScreenSeparateLine::SeparateType;
	using TriangleType = SlideTriangle::TriangleType;

	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

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

		m_spriteBox.at(sc_i(SpriteKind::White))->Render(BasePoint::Center, m_reviewScreenPos, m_reviewScreenSize, 0.0f, m_reviewScreenColor);
		//ページ数描画
		{
			if (m_chapter && m_chapter->GetSlides().size())
			{
				int _slideNum = m_slideIndex + 1;//今のページ数
				int _tenRate = 1;//今のページ数が何桁かをこの変数に入れる
				for (int i = 1; i * 10 <= _slideNum; i *= 10)
				{
					_tenRate++;
				}
				XMFLOAT2 _slideNumPos = m_reviewScreenLeftTopPos;
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

		//看板(m_signBoard)が一つでも登録されていれば
		if (m_signBoards.size())
		{
			////描画(背景のボード)処理を行う
			//for (int i = 0; i < m_signBoards.size(); i++)
			//{
			//	m_signBoards.at(i)->BoardRender();
			//}
			////描画(テキスト)処理を行う
			//for (int i = 0; i < m_signBoards.size(); i++)
			//{
			//	m_signBoards.at(i)->TextRender();
			//}
			for (int i = 0; i < m_signBoards.size(); i++)
			{
				m_signBoards.at(i)->ToolRender(m_reviewFullScreenTestFlag);
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
	if (!m_reviewFullScreenTestFlag)
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
			if (m_slideIndex > 0)
			{
				m_spriteBox.at(sc_i(SpriteKind::Triangle))->Render(BasePoint::Center, m_slideTriangle.position[sc_i(TriangleType::Left)], m_slideTriangle.size,
					0.0f, { 1.0f,1.0f,1.0f,m_slideTriangle.touchFlag[sc_i(TriangleType::Left)] ? 1.0f : 0.5f });
			}
			int _slideNum = m_slideIndex + 1;
			m_spriteBox.at(sc_i(SpriteKind::Triangle))->Render(BasePoint::Center, m_slideTriangle.position[sc_i(TriangleType::Right)], { -m_slideTriangle.size.x,m_slideTriangle.size.y },
				0.0f, { 1.0f,1.0f,m_chapter->GetSlides().size() == _slideNum ? 0.3f : 1.0f,m_slideTriangle.touchFlag[sc_i(TriangleType::Right)] ? 1.0f : 0.5f });
		}
		//ゴミ箱アイコン描画
		{
			m_spriteBox.at(sc_i(SpriteKind::DustBox))->Render(BasePoint::RightBottom, m_dustBoxPos, m_dustBoxSize,
				0.0f, { 1.0f,m_chapter->GetSlides().size() > 1 ? 1.0f : 0.3f,m_chapter->GetSlides().size() > 1 ? 1.0f : 0.3f,m_dustBoxTouchFlag ? 1.0f : 0.5f });
		}
		//Addアイコン描画
		{
			m_spriteBox.at(sc_i(SpriteKind::AddIcon))->Render(BasePoint::RightTop, m_addIconPos, m_addIconSize,
				0.0f, { 1.0f,1.0f,1.0f,m_addIconTouchFlag ? 1.0f : 0.5f });
		}
	}

}

#ifdef USE_IMGUI
//ImGui描画関数
void SceneTool::ImGuiRender()
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	//最大スケールでレビュー画面が表示されているなら終わり
	if (m_reviewFullScreenTestFlag)return;

	ImGuizmo();
	ImGuiAssetsWindow(ImGui::GetWindowSize().x * 0.85f);
	ImGuiSlideWindow();
	XMFLOAT2 _windowPos = { 0.0f,_graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	XMFLOAT2 _windowSize = { _graphics.GetScreenWidth() * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
		_graphics.GetScreenHeight() * (1.0f - m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)]) - m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	Scene::CommonImGuiRender(_windowPos, _windowSize, _flags);
}
//Assetウィンドウ用のImGui描画関数
void SceneTool::ImGuiAssetsWindow(float a_buttonWidth)
{
	Graphics& _graphics = Graphics::Instance();

	using SeparateType = ScreenSeparateLine::SeparateType;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	XMFLOAT2 _windowSize = { _graphics.GetScreenWidth() * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f,
		_graphics.GetScreenHeight() * m_screenSeparateLine.rate[sc_i(SeparateType::LeftHorizontal)] - m_screenSeparateLine.lineSize[sc_i(SeparateType::LeftHorizontal)].y * 0.5f };
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.7f, 0.7f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.9f, 0.9f, 0.0f, 1.0f));
	ImGui::Begin("Asset", nullptr, _flags);
	{
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.6f, 0.3f, 0.3f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.8f, 0.4f, 0.4f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.8f, 0.4f, 0.4f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("TextWindow", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiTextWindow(_windowSize.x * 0.85f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.3f, 0.5f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.3f, 0.3f, 0.8f, 1.0f)); //色変更
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.8f, 1.0f)); //色変更
		if (ImGui::CollapsingHeader("All Character Window", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGuiAllCharactersWindow(_windowSize.x * 0.4f);
			ImGui::Separator();
		}
		ImGui::PopStyleColor(3);	//色変更終了処理

		//ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.3f, 1.0f)); //色変更
		//ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.3f, 0.8f, 0.8f, 1.0f)); //色変更
		//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f)); //色変更
		//if (ImGui::CollapsingHeader("Select Character Window", ImGuiTreeNodeFlags_DefaultOpen))
		//{
		//	if (m_sceneEvent)
		//	{
		//		float _imageLimitHeight = _graphics.GetScreenHeight() * 0.15f;
		//		for (auto& _character : m_sceneEvent->m_activeCharacters)
		//		{
		//			ImVec2 _imageSize = { a_buttonWidth, a_buttonWidth * _character->GetSprites().at(0)->GetAspectRation() };
		//			if (_imageSize.y > _imageLimitHeight)
		//			{
		//				_imageSize.y = _imageLimitHeight;
		//				_imageSize.x = _imageSize.y / _character->GetSprites().at(0)->GetAspectRation();
		//			}
		//			ImGui::Image(_character->GetSprites().at(0)->GetShaderResource(), _imageSize);
		//		}
		//	}
		//	ImGui::Separator();
		//}
		//ImGui::PopStyleColor(3);	//色変更終了処理
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

	if (m_signBoards.size())
	{
		ImVec2 _windowSize = ImGui::GetWindowSize();	//現在のImGuiのウィンドウのサイズ取得
		//レビュー描画用の変数(座標)
		for (int i = 0; i < m_signBoards.size(); i++)
		{
			if (ImGui::TreeNodeEx(("Board : " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				float _boardSpriteScaleX = 0.6f;
				XMFLOAT2 _imageDrawSize = { _windowSize.x * _boardSpriteScaleX ,_windowSize.x * _boardSpriteScaleX * m_signBoards.at(i)->GetBoardSprite()->GetAspectRation() };
				//もし、縦のサイズが大きすぎたら、
				if (_imageDrawSize.y > _graphics.GetScreenHeight() * 0.2f)
				{
					float _boardSpriteScaleY = 0.15f;
					//縦のサイズを補正して、それに応じた横のサイズを設定する
					_imageDrawSize.y = _graphics.GetScreenHeight() * _boardSpriteScaleY;
					_imageDrawSize.x = _imageDrawSize.y / m_signBoards.at(i)->GetBoardSprite()->GetAspectRation();
				}
				//レビュー描画
				ImGui::Image(m_signBoards.at(i)->GetBoardSprite()->GetShaderResource(), ImVec2(_imageDrawSize.x, _imageDrawSize.y));
				ImGui::Separator();

				ImGui::Text("Text");
				ImGui::InputTextMultiline(("##" + std::to_string(i)).c_str(),  // ラベル
					m_signBoards.at(i)->m_inputBuffer,           // テキストバッファ
					IM_ARRAYSIZE(m_signBoards.at(i)->m_inputBuffer), // バッファサイズ
					ImVec2(ImGui::GetWindowSize().x * 0.75f, ImGui::GetWindowSize().y * 0.05f));


				/*wchar_t _wideBuffer[256] = {};
				MultiByteToWideChar(CP_UTF8, 0, m_signBoards.at(i)->m_inputBuffer, -1, _wideBuffer, 256);
				std::string sihsh = ConvertWideToUtf8(_wideBuffer);
				m_signBoards.at(i)->jElements.text = ConvertWideToUtf8(_wideBuffer);*/

				/*wchar_t _wideBuffer[256] = {};
				int result = MultiByteToWideChar(CP_UTF8, 0, m_signBoards.at(i)->m_inputBuffer, -1, _wideBuffer, 256);
				if (result > 0)
				{
					std::string utf8Text = ConvertWideToUtf8(_wideBuffer);
					m_signBoards.at(i)->jElements.text = utf8Text;
				}*/

				ImGui::TreePop();
			}
		}
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
			m_signBoards.emplace_back(std::make_unique<SignBoard>(_device, _deviceContext, _jsonFilePath.string()));
			m_signBoards.at(m_signBoards.size() - 1)->jElements.text = "こんばんは\nこんばんは\nおはよう";
		}
	}
	//m_textWindowがnullptrじゃなかったら
	if (m_signBoards.size())
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
		//Resetボタン(押すとtextWindowポインタをnullptrにする)
		if (ImGui::Button("Reset Text Window", ImVec2(a_buttonWidth * 0.5f, 0.0f)))
		{
			m_signBoards.clear();
		}
		ImGui::PopStyleColor(2);
	}
}

void SceneTool::ImGuiAllCharactersWindow(float a_buttonWidth)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();
	ID3D11DeviceContext* _deviceContext = _graphics.GetDeviceContext();
	AllFacialSet& _allFacIns = AllFacialSet::Instance();
	auto _allFacialSet = _allFacIns.GetAllFacialSet();

	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

	std::vector<std::string> _characterKeys{};
	std::vector<const char*> _cstrItems;
	static int _currentItem = 0;
	float _imageLimitHeight = _screenSize.y * 0.1f;
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

		ImGui::Text(_cstrItems.at(0));

		Sprite* _characterSprite = _allFacIns.GetFacialSet(_currentItem)->GetFacial(0).get();
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

		auto& appearingCharacters = m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters();
		if (std::none_of(appearingCharacters.begin(), appearingCharacters.end(),
			[&](const auto& c) { return c->name == _facialSetItr->first; }))
		{
			appearingCharacters.emplace_back(std::make_shared<Character>(_facialSetItr->first, _facialSetItr->second));
		}
	}
}

void SceneTool::ImGuiSlideWindow()
{
	ImGuiCharactersWindow();
	ImGuiEnterWindow();
	ImGuiExcuteWindow();
	ImGuiExitWindow();
}

void SceneTool::ImGuiCharactersWindow()
{
	Graphics& _graphics = Graphics::Instance();
	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };
	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_charactersWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(_screenSize.x * m_screenSeparateLine.rate[sc_i(SeparateType::Vertical)] + m_screenSeparateLine.lineSize[sc_i(SeparateType::Vertical)].x * 0.5f, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.6f, 0.15f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.3f, 1.0f, 1.0f));
	ImGui::Begin("Characters", nullptr, _flags);
	{
		if (m_chapter)
		{
			if (m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().size())
			{
				std::vector<std::string> _facialsKeys{};
				std::vector<const char*> _cstrItems;
				float _spriteSizeX = ImGui::GetWindowSize().x * 0.5f;
				XMFLOAT2 _imageLimitSize = { _screenSize.x * 0.1f, _screenSize.y * 0.1f };
				for (int i = 0, iEnd = m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().size(); i < iEnd; i++)
				{
					_facialsKeys.clear();
					_cstrItems.clear();
					for (auto& _key : m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().at(i)->GetFacialSet()->keys)
					{
						_facialsKeys.emplace_back(_key);	// ← ここで _facialsKeys が文字列の実体を保持
					}
					for (const auto& str : _facialsKeys)
					{
						_cstrItems.push_back(str.c_str());	// ← 安全にポインタを取得
					}
					std::string _comboLabel = "##" + std::to_string(i) + "Facial";
					ImGui::Combo(_comboLabel.c_str(), &m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().at(i)->facialIndex, _cstrItems.data(), static_cast<int>(_cstrItems.size()));

					int _index = m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().at(i)->facialIndex;
					Sprite* _facialSprite = m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().at(i)->GetFacialSet()->GetFacial(_index).get();
					ImVec2 _imageSize = { _imageLimitSize.x, _imageLimitSize.x * _facialSprite->GetAspectRation() };
					if (_imageSize.y > _imageLimitSize.y)
					{
						_imageSize.y = _imageLimitSize.y;
						_imageSize.x = _imageSize.y / _facialSprite->GetAspectRation();
					}
					ImGui::Image(_facialSprite->GetShaderResource(), _imageSize);

					if (ImGui::Button("Delete Character", ImVec2(_spriteSizeX, 0.0f)))
					{
						m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().erase(m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().begin() + i);
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
	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.5f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.3f, 0.3f, 1.0f));
	ImGui::Begin("Enter Window", nullptr, _flags);
	{

	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiExcuteWindow()
{
	Graphics& _graphics = Graphics::Instance();
	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x + m_eventWindowWidth, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.15f, 0.5f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
	ImGui::Begin("Excute Window", nullptr, _flags);
	{

	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuiExitWindow()
{
	Graphics& _graphics = Graphics::Instance();
	//画面サイズ
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

	using SeparateType = ScreenSeparateLine::SeparateType;

	XMFLOAT2 _windowSize = { m_eventWindowWidth,m_slideWindowHeight };
	ImGui::SetNextWindowPos(ImVec2(m_eventWindowDrawStartPos.x + m_eventWindowWidth * 2.0f, m_eventWindowDrawStartPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(_windowSize.x, _windowSize.y), ImGuiCond_Always);
	ImGuiWindowFlags _flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.15f, 0.15f, 0.5f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.3f, 0.3f, 0.7f, 1.0f));
	ImGui::Begin("Exit Window", nullptr, _flags);
	{

	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}

void SceneTool::ImGuizmo()
{
	Graphics& _graphics = Graphics::Instance();

	if (!m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters().size())return;

	if (ImGui::TreeNodeEx("Guozmo", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNodeEx("Guizmo Mode", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::RadioButton("TRANSLATE", &m_guizmoType, sc_i(ImGuizmo::OPERATION::TRANSLATE));
			ImGui::SameLine();
			ImGui::RadioButton("SCALE", &m_guizmoType, sc_i(ImGuizmo::OPERATION::SCALE));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	//Guizmoの作業領域
	ImVec2 _guizmoTaskPos = {}, _guizmoTaskSize = {};
	ImGuizmo::SetOrthographic(true);
	XMFLOAT4X4 _guizmoTransform;
	DirectX::XMMATRIX _selectS{}, _selectR{}, _selectT{};
	_selectR = DirectX::XMMatrixRotationZ(0.0f);

	_guizmoTaskPos = { m_reviewScreenLeftTopPos.x,m_reviewScreenLeftTopPos.y };
	_guizmoTaskSize = { m_reviewScreenSize.x,m_reviewScreenSize.y };
	_selectS = DirectX::XMMatrixScaling(m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.x, m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.y, 1.0f);
	_selectT = DirectX::XMMatrixTranslation(m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.x, m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.y, 0.0f);
	
	/*switch (static_cast<SelectObject>(m_editElements.selectObjectIndex))
	{
	case SelectObject::Board:
		_guizmoTaskPos = { m_reviewBoard.leftTopPos.x,m_reviewBoard.leftTopPos.y };
		_guizmoTaskSize = { m_reviewBoard.size.x,m_reviewBoard.size.y };
		_selectS = DirectX::XMMatrixScaling(m_editElements.jElements.boardLocalSize.x, m_editElements.jElements.boardLocalSize.y, 1.0f);
		_selectT = DirectX::XMMatrixTranslation(m_editElements.jElements.boardLocalPosition.x, m_editElements.jElements.boardLocalPosition.y, 0.0f);
		break;
	case SelectObject::DrawableArea:
		XMFLOAT2 _boardLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_editElements.jElements.boardPosition, m_editElements.jElements.boardSize);
		_guizmoTaskPos = { _boardLeftTopPos.x,_boardLeftTopPos.y };
		_guizmoTaskSize = { m_editElements.jElements.boardSize.x,m_editElements.jElements.boardSize.y };
		_selectS = DirectX::XMMatrixScaling(m_editElements.jElements.textDrawableAreaLocalSize.x, m_editElements.jElements.textDrawableAreaLocalSize.y, 1.0f);
		_selectT = DirectX::XMMatrixTranslation(m_editElements.jElements.textDrawableAreaLocalPosition.x, m_editElements.jElements.textDrawableAreaLocalPosition.y, 0.0f);
		break;
	case SelectObject::TextStartPosition:
		XMFLOAT2 _textDrawableAreaLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, m_editElements.jElements.textDrawableAreaPosition, m_editElements.jElements.textDrawableAreaSize);
		_guizmoTaskPos = { _textDrawableAreaLeftTopPos.x,_textDrawableAreaLeftTopPos.y };
		_guizmoTaskSize = { m_editElements.jElements.textDrawableAreaSize.x,m_editElements.jElements.textDrawableAreaSize.y };
		_selectS = DirectX::XMMatrixScaling(1.0f, m_editElements.jElements.textLocalHeight, 1.0f);
		_selectT = DirectX::XMMatrixTranslation(m_editElements.jElements.textDrawStartLocalPosition.x, m_editElements.jElements.textDrawStartLocalPosition.y, 0.0f);
		break;
	}*/
	ImGuizmo::SetRect(_guizmoTaskPos.x, _guizmoTaskPos.y, _guizmoTaskSize.x, _guizmoTaskSize.y);
	DirectX::XMStoreFloat4x4(&_guizmoTransform, _selectS * _selectR * _selectT);

	//行列用の変数
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	float view_mat[16], proj_mat[16];
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)view_mat, view);
	DirectX::XMStoreFloat4x4((XMFLOAT4X4*)proj_mat, proj);

	ImGuizmo::Manipulate(view_mat, proj_mat, static_cast<ImGuizmo::OPERATION>(m_guizmoType), ImGuizmo::LOCAL, &_guizmoTransform._11);
	if (ImGuizmo::IsUsing())
	{
		XMVECTOR _scale, _rotation, _translation;
		XMMATRIX _newTransform = XMLoadFloat4x4(&_guizmoTransform);
		XMMatrixDecompose(&_scale, &_rotation, &_translation, _newTransform);

		XMStoreFloat2(&m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize, _scale);
		m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.x = std::clamp(
			m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.x, FLT_EPSILON, 5.0f);
		m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.y = std::clamp(
			m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizeSize.y, FLT_EPSILON, 5.0f);

		XMStoreFloat2(&m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition, _translation);
		m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.x = std::clamp(
			m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.x, FLT_EPSILON, 1.0f);
		m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.y = std::clamp(
			m_chapter->GetSlides()[m_slideIndex].GetAppearingCharacters()[0]->normalizePosition.y, FLT_EPSILON, 1.0f);
	}
}

//void SceneTool::TextWindowEditor(float a_buttonWidth)
//{
//	Graphics& _graphics = Graphics::Instance();
//
//	using SeparateType = ScreenSeparateLine::SeparateType;
//
//	//m_textWindowがnullptrじゃなかったら
//	if (m_signBoards.size())
//	{
//		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
//		//Resetボタン(押すとtextWindowポインタをnullptrにする)
//		if (ImGui::Button("Reset Text Window", ImVec2(a_buttonWidth, 0.0f)))
//		{
//			m_signBoards.clear();
//		}
//		ImGui::PopStyleColor(1);
//	}
//
//	if (ImGui::Button("Select TextWindow Json File", ImVec2(a_buttonWidth, 0.0f)))
//	{
//		static const char* filter = "Model Files(*.json;)\0*.json;\0All Files(*.*)\0*.*;\0\0";
//
//		char _selectFileName[256] = { 0 };
//		HWND _hWnd = Graphics::Instance().GetHWND();
//		DialogResult _result = Dialog::OpenFileName(_selectFileName, sizeof(_selectFileName), filter, nullptr, _hWnd);
//		if (_result == DialogResult::OK)
//		{
//			wchar_t _wideBuffer[256]; ////選択したファイルのパスを保存する(wchar_t <--- char)
//			MultiByteToWideChar(CP_ACP, 0, _selectFileName, -1, _wideBuffer, 256); //(wchar_t <--- char)
//			//絶対パスを相対パス化
//			auto _jsonFilePath = std::filesystem::relative(_wideBuffer);
//			m_signBoards.emplace_back(std::make_unique<SignBoard>(_device, _deviceContext, _jsonFilePath.string()));
//			m_signBoards.at(m_signBoards.size() - 1)->jElements.text = "こんばんは\nこんばんは\nおはよう";
//		}
//	}
//	ImGui::Separator(); ImGui::Separator(); ImGui::Separator();
//
//#ifdef _DEBUG
//	if (ImGui::CollapsingHeader("Debug Element", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//
//	}
//#endif
//}
#endif // USE_IMGUI

void SceneTool::OnSizeChange()
{
	Graphics& _graphics = Graphics::Instance();
}

void SceneTool::MouseVsTriangles()
{
	Mouse& _mouse = Input::Instance().GetMouse();

	using TriangleType = SlideTriangle::TriangleType;

	XMFLOAT2 _leftTopPos;
	XMFLOAT2 _rightBottomPos;

	auto _TouchAndHoldCheck = [&](int a_type, BasePoint a_basePoint)
		{
			_leftTopPos = CalcSquareLeftTopPosition(a_basePoint, m_slideTriangle.position[a_type], m_slideTriangle.size);
			_rightBottomPos = CalcSquareRightBottomPosition(a_basePoint, m_slideTriangle.position[a_type], m_slideTriangle.size);
			if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
				_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
			{
				m_slideTriangle.touchFlag[a_type] = false;
			}
			else
			{
				m_slideTriangle.touchFlag[a_type] = true;
			}
		};

	_TouchAndHoldCheck(sc_i(TriangleType::Left), BasePoint::Center);
	_TouchAndHoldCheck(sc_i(TriangleType::Right), BasePoint::Center);

	if (m_slideTriangle.touchFlag[sc_i(TriangleType::Left)])
	{
		if (m_slideIndex > 0 && _mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			m_slideIndex--;
		}
	}
	else if (m_slideTriangle.touchFlag[sc_i(TriangleType::Right)])
	{
		if (_mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			m_slideIndex++;
			if (m_chapter->GetSlides().size() == m_slideIndex)
			{
				m_chapter->GetSlides().emplace_back(Slide());
			}
		}
	}
}

void SceneTool::MouseVsDustBox()
{
	Mouse& _mouse = Input::Instance().GetMouse();

	XMFLOAT2 _leftTopPos = CalcSquareLeftTopPosition(BasePoint::RightBottom, m_dustBoxPos, m_dustBoxSize);
	XMFLOAT2 _rightBottomPos = CalcSquareRightBottomPosition(BasePoint::RightBottom, m_dustBoxPos, m_dustBoxSize);
	if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
		_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
	{
		m_dustBoxTouchFlag = false;
	}
	else
	{
		m_dustBoxTouchFlag = true;
	}

	if (m_dustBoxTouchFlag)
	{
		if (m_chapter->GetSlides().size() > 1 && _mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			m_chapter->GetSlides().erase(m_chapter->GetSlides().begin() + m_slideIndex);
			if (m_slideIndex >= m_chapter->GetSlides().size())
			{
				m_slideIndex = m_chapter->GetSlides().size() - 1;
			}
		}
	}
}

void SceneTool::MouseVsAddIcon()
{
	Mouse& _mouse = Input::Instance().GetMouse();

	XMFLOAT2 _leftTopPos = CalcSquareLeftTopPosition(BasePoint::RightTop, m_addIconPos, m_addIconSize);
	XMFLOAT2 _rightBottomPos = CalcSquareRightBottomPosition(BasePoint::RightTop, m_addIconPos, m_addIconSize);
	if (_mouse.GetPositionX() < _leftTopPos.x || _rightBottomPos.x < _mouse.GetPositionX() ||
		_mouse.GetPositionY() < _leftTopPos.y || _rightBottomPos.y < _mouse.GetPositionY())
	{
		m_addIconTouchFlag = false;
	}
	else
	{
		m_addIconTouchFlag = true;
	}

	if (m_addIconTouchFlag)
	{
		if (_mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			m_chapter->GetSlides().insert(m_chapter->GetSlides().begin() + m_slideIndex + 1, Slide());
			m_slideIndex++;
		}
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
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };
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