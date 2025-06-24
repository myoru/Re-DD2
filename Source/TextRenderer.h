#pragma once					

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "Sprite.h"
#include "Timer.h"

using namespace DirectX;

class TextRenderer
{
public:
	TextRenderer(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, const wchar_t* a_fileName);
	~TextRenderer() {}

	enum class TextAlignment
	{
		Leading,
		Center,
		Trailing,
	};

	bool Update(const wchar_t* a_text, int a_drawableTextLen);
	void Begin();
	void Render(XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize,
		TextAlignment a_textAlignment = TextAlignment::Leading, float a_lineSpace = 0.0f, FXMVECTOR a_textColor = Colors::White, float a_scale = 1.0f);
	void ToolRender(XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize, TextAlignment a_textAlignment = TextAlignment::Leading, float a_lineSpace = 0.0f,
		float a_rotation = 0.0f, FXMVECTOR a_textColor = Colors::White, float a_scale = 1.0f, bool a_fullScreenTestFlag = false);
	void ToolRender2(std::wstring a_text, XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize, TextAlignment a_textAlignment = TextAlignment::Leading, float a_lineSpace = 0.0f,
		float a_rotation = 0.0f, FXMVECTOR a_textColor = Colors::White, float a_scale = 1.0f, bool a_fullScreenTestFlag = false);
	void End();

	XMFLOAT2 GetFontSize() { return m_fontSize; }
private:
	std::unique_ptr<SpriteBatch> m_spriteBatch;
	std::unique_ptr<SpriteFont> m_spriteFont;
	std::wstring m_drawText; //const wchar_t*‚ðwstring‚ÉŠi”[‚·‚é
	int m_textLen = 0; //‰½•¶Žš‚©
	int m_drawableTextLen = 0; //‰½•¶Žš•`‰æ‚·‚é‚©
	XMFLOAT2 m_fontSize = {};
	bool m_endIsNewline = false;
};
