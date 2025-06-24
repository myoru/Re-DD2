#include "SignBoard.h"
#include "Graphics.h"
#include "ImGuiPack.h"
#ifdef USE_IMGUI
#include "../External/imgui/ImGuizmo.h"
#endif

using namespace DirectX;

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

std::wstring ToWString(const std::string& a_str)
{
	int _len = MultiByteToWideChar(CP_UTF8, 0, a_str.c_str(), -1, nullptr, 0);
	std::wstring _wstr(_len, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, a_str.c_str(), -1, &_wstr[0], _len);
	return _wstr;
}
std::string WideToUtf8(const std::wstring& wideStr)
{
	if (wideStr.empty()) return std::string();

	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(sizeNeeded - 1, 0); // 終端分を除く
	WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);

	return result;
}

SignBoard::SignBoard(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, std::string a_jsonFileName)
{
	Graphics& _graphics = Graphics::Instance();

	LoadJson(a_jsonFileName.c_str(), jElements);

	m_boardSpr = std::make_unique<Sprite>(ToWString(jElements.boardFilePath).c_str(), BasePoint::Center);
	m_textRenderer = std::make_unique<TextRenderer>(a_device, a_deviceContext, ToWString(jElements.fontFilePath).c_str());
}

SignBoard::~SignBoard()
{
}

void SignBoard::Update(float a_elapsedTime, char* a_textBuffer, int a_drawableTextLen, DirectX::XMFLOAT2 a_screenLeftTopPosition, DirectX::XMFLOAT2 a_screenSize)
{
	Graphics& _graphics = Graphics::Instance();
	XMFLOAT2 _screenSize = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };
	if (a_screenSize.x >= 0.0f)
	{
		_screenSize = a_screenSize;
	}
	XMFLOAT2 _screenCenterPos = { _screenSize.x * 0.5f,_screenSize.y * 0.5f };

	//ボードのパラメーター更新
	{
		jElements.boardSize.x = _screenSize.x * jElements.boardLocalSize.x;
		jElements.boardSize.y = _screenSize.y * jElements.boardLocalSize.y;
		jElements.boardPosition.x = a_screenLeftTopPosition.x + _screenSize.x * jElements.boardLocalPosition.x;
		jElements.boardPosition.y = a_screenLeftTopPosition.y + _screenSize.y * jElements.boardLocalPosition.y;
	}

	//テキスト描画可能エリアのパラメーター更新
	{
		//テキストウィンドウの左上の座標を取得
		XMFLOAT2 _boardLeftTopPosition = CalcSquareLeftTopPosition(BasePoint::Center, jElements.boardPosition, jElements.boardSize);

		jElements.textDrawableAreaSize.x = jElements.boardSize.x * jElements.textDrawableAreaLocalSize.x;
		jElements.textDrawableAreaSize.y = jElements.boardSize.y * jElements.textDrawableAreaLocalSize.y;
		jElements.textDrawableAreaPosition.x = _boardLeftTopPosition.x + (jElements.boardSize.x * jElements.textDrawableAreaLocalPosition.x);
		jElements.textDrawableAreaPosition.y = _boardLeftTopPosition.y + (jElements.boardSize.y * jElements.textDrawableAreaLocalPosition.y);
	}

	//テキスト描画開始位置のパラメーター更新
	{
		//テキスト描画が可能なエリアの左上、および右下の座標を取得
		XMFLOAT2 _textDrawableAreaLeftTopPosition = CalcSquareLeftTopPosition(BasePoint::Center, jElements.textDrawableAreaPosition, jElements.textDrawableAreaSize);
		XMFLOAT2 _textDrawableAreaRightBottomPosition = CalcSquareRightBottomPosition(BasePoint::Center, jElements.textDrawableAreaPosition, jElements.textDrawableAreaSize);

		jElements.textScale = (jElements.textDrawableAreaSize.y * jElements.textLocalHeight) / m_textRenderer->GetFontSize().y;
		//アライメントに応じたテキスト描画開始位置のＸ成分を計算
		switch (static_cast<TextRenderer::TextAlignment>(jElements.textAlignment))
		{
		case TextRenderer::TextAlignment::Leading:
			jElements.textDrawStartPosition.x = _textDrawableAreaLeftTopPosition.x; break;
		case TextRenderer::TextAlignment::Center:
			jElements.textDrawStartPosition.x = jElements.textDrawableAreaPosition.x; break;
		case TextRenderer::TextAlignment::Trailing:
			jElements.textDrawStartPosition.x = _textDrawableAreaRightBottomPosition.x; break;
		}
		//テキスト描画開始位置のｙ成分を計算
		jElements.textDrawStartPosition.y = _textDrawableAreaLeftTopPosition.y + (jElements.textDrawableAreaSize.y * jElements.textDrawStartLocalPosition.y);

		jElements.lineSpace = jElements.textDrawableAreaSize.y * jElements.localLineSpace;
	}
	wchar_t _wideBuffer[256] = {};
	MultiByteToWideChar(CP_UTF8, 0, a_textBuffer, -1, _wideBuffer, 256);

	m_textRenderer->Update(_wideBuffer, a_drawableTextLen);
}

void SignBoard::BoardRender()
{
	m_boardSpr->Render(BasePoint::Center, jElements.boardPosition, jElements.boardSize);
}

void SignBoard::TextRender(bool a_fullscreen, FXMVECTOR a_textColor)
{
	m_textRenderer->Begin();
	XMFLOAT2 _textDrawableAreaLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, jElements.textDrawableAreaPosition, jElements.textDrawableAreaSize);
	m_textRenderer->ToolRender(jElements.textDrawStartPosition, _textDrawableAreaLeftTopPos, jElements.textDrawableAreaSize,
		static_cast<TextRenderer::TextAlignment>(jElements.textAlignment), jElements.lineSpace, 0.0f, a_textColor, jElements.textScale, a_fullscreen);
	m_textRenderer->End();
}

void SignBoard::ToolRender(bool a_fullscreen, FXMVECTOR a_textColor)
{
	/*m_boardSpr->Render(BasePoint::Center, jElements.boardPosition, jElements.boardSize);

	wchar_t _wideBuffer[256] = {};
	MultiByteToWideChar(CP_UTF8, 0, m_inputBuffer, -1, _wideBuffer, 256);

	m_textRenderer->Begin();
	XMFLOAT2 _textDrawableAreaLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, jElements.textDrawableAreaPosition, jElements.textDrawableAreaSize);
	m_textRenderer->ToolRender(_wideBuffer, jElements.textDrawStartPosition, _textDrawableAreaLeftTopPos, jElements.textDrawableAreaSize,
		static_cast<TextRenderer::TextAlignment>(jElements.textAlignment), jElements.lineSpace, 0.0f, a_textColor, jElements.textScale, a_fullscreen);
	m_textRenderer->End();*/
}

//void SignBoard::ToolRender2(bool a_fullscreen, FXMVECTOR a_textColor)
//{
//	m_boardSpr->Render(BasePoint::Center, jElements.boardPosition, jElements.boardSize);
//
//	m_textRenderer->Begin();
//	XMFLOAT2 _textDrawableAreaLeftTopPos = CalcSquareLeftTopPosition(BasePoint::Center, jElements.textDrawableAreaPosition, jElements.textDrawableAreaSize);
//	m_textRenderer->ToolRender2(jElements.wText, jElements.textDrawStartPosition, _textDrawableAreaLeftTopPos, jElements.textDrawableAreaSize,
//		static_cast<TextRenderer::TextAlignment>(jElements.textAlignment), jElements.lineSpace, 0.0f, a_textColor, jElements.textScale, a_fullscreen);
//	m_textRenderer->End();
//}

#ifdef USE_IMGUI

#endif