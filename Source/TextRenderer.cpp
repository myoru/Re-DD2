#include "TextRenderer.h"
#include "StringConvert.h"

TextRenderer::TextRenderer(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, const wchar_t* a_fileName)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(a_deviceContext);
	m_spriteFont = std::make_unique<SpriteFont>(a_device, a_fileName);
	DirectX::XMStoreFloat2(&m_fontSize, m_spriteFont->MeasureString(L"◇"));
}

void TextRenderer::Update(float a_elapsedTime)
{
}

void TextRenderer::Begin()
{
	m_spriteBatch->Begin();
}

void TextRenderer::Render(std::string a_text, XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize, TextAlignment a_textAlignment, float a_lineSpace, FXMVECTOR a_textColor, float a_scale)
{
	std::wstring _text = ShiftJisToWstring(a_text);	//const wchar_t*をwstringに格納する
	int _textLen = _text.length();	//何文字か
	std::vector<std::wstring> _textParts;	//改行が行われるごとにこのコンテナに追加していく(改行無しの場合でも最低でも要素が1つは存在する設計)

	//もし文字が入力されていたら
	if (_textLen)
	{
		std::wstring _textPart = {};	//改行されるまでの文字列をここにどんどん格納していく
		DirectX::XMFLOAT2 _textPartSize{};	//文字を描画した際のサイズを格納する(スクリーン空間)
		for (int i = 0; i < _textLen; i++)
		{
			//m_endsInNewlineの初期化
			m_endIsNewline = false;
			//コピー(次の文字を格納しても描画可能範囲から溢れないかをチェックするためにコピーする)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += _text.at(i);	//継続条件が[i < _textLen]であるため問題なく指定要素を取得できる
			//文字列の長さを取得
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//もし、描画可能範囲から溢れていたら
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				//文字列を格納
				_textPart += _text.at(i);
			}
			//改行の入力がされていたら
			else if (_text.at(i) == L'\n')
			{
				//改行入力(\n)は格納し無くてよいので、_textPart += str.at(i)を行わない
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//文字列を格納
				_textPart += _text.at(i);
			}
			if (a_drawStartPosition.y - a_drawableAreaLeftTopPosition.y + (_textParts.size() + 1) * (m_fontSize.y * a_scale) + a_lineSpace * _textParts.size() > a_drawableAreaSize.y)
				break;
			if (_textLen == i + 1)
			{
				if (_textPart.size())
				{
					_textParts.emplace_back(_textPart);
				}
				break;
			}
		}
	}



	float _forecastPointOffset = {};
	float _forecastSize = m_fontSize.y * a_scale;
	switch (a_textAlignment)
	{
	case TextRenderer::TextAlignment::Center://中央揃え
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://右端揃え
		_forecastPointOffset = (m_fontSize.y * a_scale);
		break;
	default:
		break;
	}

	DirectX::XMFLOAT2 _origin = {};
	DirectX::XMFLOAT2 _textPartSize;
	float _neztDrawMarkerWidth = 5.0f;
	if (_textParts.size())
	{
		for (int i = 0; i < _textParts.size(); i++)
		{
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_textParts.at(i).c_str()));
			_origin = {};
			switch (a_textAlignment)
			{
			case TextRenderer::TextAlignment::Leading://左端揃え
				break;
			case TextRenderer::TextAlignment::Center://中央揃え
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://右端揃え
				_origin.x = _textPartSize.x;
				break;
			default:
				break;
			}
			_textPartSize.x *= a_scale; _textPartSize.y *= a_scale;
			m_spriteFont->DrawString(m_spriteBatch.get(), _textParts.at(i).c_str(), { a_drawStartPosition.x , a_drawStartPosition.y + i * (_textPartSize.y + a_lineSpace) },
				a_textColor, 0.0f, _origin, a_scale);
		}
		XMFLOAT2 _nextDrawPosition = { a_drawStartPosition.x ,0.0f };
		if (m_endIsNewline)
		{
			_nextDrawPosition.y = a_drawStartPosition.y + (_textParts.size()) * (m_fontSize.y * a_scale + a_lineSpace);
		}
		else
		{
			switch (a_textAlignment)
			{
			case TextRenderer::TextAlignment::Leading://左端揃え
				_nextDrawPosition.x += DirectX::XMVectorGetX(m_spriteFont->MeasureString(_textParts.at(_textParts.size() - 1).c_str())) * a_scale;
				break;
			case TextRenderer::TextAlignment::Center://中央揃え
				_nextDrawPosition.x += (DirectX::XMVectorGetX(m_spriteFont->MeasureString(_textParts.at(_textParts.size() - 1).c_str())) * 0.5f) * a_scale;
				break;
			default:
				break;
			}
			_nextDrawPosition.y = a_drawStartPosition.y + (_textParts.size() - 1) * (m_fontSize.y * a_scale + a_lineSpace);
		}
	}
}

void TextRenderer::ToolRender(const wchar_t* a_text, DirectX::XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize,
	TextAlignment a_textAlignment, float a_lineSpace, float a_rotation, DirectX::FXMVECTOR a_textColor, float a_scale, bool a_fullScreenTestFlag)
{
	int _textLen = std::wcslen(a_text);		//何文字か
	std::wstring str(a_text);	//const wchar_t*をwstringに格納する
	std::vector<std::wstring> _textParts;	//改行が行われるごとにこのコンテナに追加していく(改行無しの場合でも最低でも要素が1つは存在する設計)

	//もし文字が入力されていたら
	if (_textLen)
	{
		std::wstring _textPart = {};	//改行されるまでの文字列をここにどんどん格納していく
		DirectX::XMFLOAT2 _textPartSize{};	//文字を描画した際のサイズを格納する(スクリーン空間)
		for (int i = 0; i < _textLen; i++)
		{
			//m_endsInNewlineの初期化
			m_endIsNewline = false;
			//コピー(次の文字を格納しても描画可能範囲から溢れないかをチェックするためにコピーする)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += str.at(i);	//継続条件が[i < _textLen]であるため問題なく指定要素を取得できる
			//文字列の長さを取得
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//もし、描画可能範囲から溢れていたら
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				//文字列を格納
				_textPart += str.at(i);
			}
			//改行の入力がされていたら
			else if (str.at(i) == L'\n')
			{
				//改行入力(\n)は格納し無くてよいので、_textPart += str.at(i)を行わない
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//文字列を格納
				_textPart += str.at(i);
			}
			if (a_drawStartPosition.y - a_drawableAreaLeftTopPosition.y + (_textParts.size() + 1) * (m_fontSize.y * a_scale) + a_lineSpace * _textParts.size() > a_drawableAreaSize.y)
				break;
			if (_textLen == i + 1)
			{
				if (_textPart.size())
				{
					_textParts.emplace_back(_textPart);
				}
				break;
			}
		}
	}



	float _forecastPointOffset = {};
	float _forecastSize = m_fontSize.y * a_scale;
	switch (a_textAlignment)
	{
	case TextRenderer::TextAlignment::Center://中央揃え
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://右端揃え
		_forecastPointOffset = (m_fontSize.y * a_scale);
		break;
	default:
		break;
	}

	DirectX::XMFLOAT2 _origin = {};
	DirectX::XMFLOAT2 _textPartSize;
	float _neztDrawMarkerWidth = 5.0f;
	if (_textParts.size())
	{
		for (int i = 0; i < _textParts.size(); i++)
		{
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_textParts.at(i).c_str()));
			_origin = {};
			switch (a_textAlignment)
			{
			case TextRenderer::TextAlignment::Leading://左端揃え
				break;
			case TextRenderer::TextAlignment::Center://中央揃え
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://右端揃え
				_origin.x = _textPartSize.x;
				break;
			default:
				break;
			}
			/*_origin.y -= m_fontSize.y / 32.0f;
			_origin.x += m_fontSize.y / 11.0f;*/
			_textPartSize.x *= a_scale; _textPartSize.y *= a_scale;
			m_spriteFont->DrawString(m_spriteBatch.get(), _textParts.at(i).c_str(), { a_drawStartPosition.x , a_drawStartPosition.y + i * (_textPartSize.y + a_lineSpace) },
				a_textColor, DirectX::XMConvertToRadians(a_rotation), _origin, a_scale);
		}
	}
}

void TextRenderer::ToolRender2(std::wstring a_text, XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize, TextAlignment a_textAlignment, float a_lineSpace, float a_rotation, FXMVECTOR a_textColor, float a_scale, bool a_fullScreenTestFlag)
{
	int _textLen = a_text.length();		//何文字か
	std::wstring str(a_text);	//const wchar_t*をwstringに格納する
	std::vector<std::wstring> _textParts;	//改行が行われるごとにこのコンテナに追加していく(改行無しの場合でも最低でも要素が1つは存在する設計)

	//もし文字が入力されていたら
	if (_textLen)
	{
		std::wstring _textPart = {};	//改行されるまでの文字列をここにどんどん格納していく
		DirectX::XMFLOAT2 _textPartSize{};	//文字を描画した際のサイズを格納する(スクリーン空間)
		for (int i = 0; i < _textLen; i++)
		{
			//m_endsInNewlineの初期化
			m_endIsNewline = false;
			//コピー(次の文字を格納しても描画可能範囲から溢れないかをチェックするためにコピーする)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += str.at(i);	//継続条件が[i < _textLen]であるため問題なく指定要素を取得できる
			//文字列の長さを取得
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//もし、描画可能範囲から溢れていたら
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				//文字列を格納
				_textPart += str.at(i);
			}
			//改行の入力がされていたら
			else if (str.at(i) == L'\n')
			{
				//改行入力(\n)は格納し無くてよいので、_textPart += str.at(i)を行わない
				//コンテナに改行されるまでの文字列データを格納
				_textParts.emplace_back(_textPart);
				//クリア
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//文字列を格納
				_textPart += str.at(i);
			}
			if (a_drawStartPosition.y - a_drawableAreaLeftTopPosition.y + (_textParts.size() + 1) * (m_fontSize.y * a_scale) + a_lineSpace * _textParts.size() > a_drawableAreaSize.y)
				break;
			if (_textLen == i + 1)
			{
				if (_textPart.size())
				{
					_textParts.emplace_back(_textPart);
				}
				break;
			}
		}
	}



	float _forecastPointOffset = {};
	float _forecastSize = m_fontSize.y * a_scale;
	switch (a_textAlignment)
	{
	case TextRenderer::TextAlignment::Center://中央揃え
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://右端揃え
		_forecastPointOffset = (m_fontSize.y * a_scale);
		break;
	default:
		break;
	}

	DirectX::XMFLOAT2 _origin = {};
	DirectX::XMFLOAT2 _textPartSize;
	float _neztDrawMarkerWidth = 5.0f;
	if (_textParts.size())
	{
		for (int i = 0; i < _textParts.size(); i++)
		{
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_textParts.at(i).c_str()));
			_origin = {};
			switch (a_textAlignment)
			{
			case TextRenderer::TextAlignment::Leading://左端揃え
				break;
			case TextRenderer::TextAlignment::Center://中央揃え
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://右端揃え
				_origin.x = _textPartSize.x;
				break;
			default:
				break;
			}
			/*_origin.y -= m_fontSize.y / 32.0f;
			_origin.x += m_fontSize.y / 11.0f;*/
			_textPartSize.x *= a_scale; _textPartSize.y *= a_scale;
			m_spriteFont->DrawString(m_spriteBatch.get(), _textParts.at(i).c_str(), { a_drawStartPosition.x , a_drawStartPosition.y + i * (_textPartSize.y + a_lineSpace) },
				a_textColor, DirectX::XMConvertToRadians(a_rotation), _origin, a_scale);
		}
	}
}

void TextRenderer::End()
{
	m_spriteBatch->End();
}