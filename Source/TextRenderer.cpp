#include "TextRenderer.h"
#include "StringConvert.h"
#include <algorithm>

TextRenderer::TextRenderer(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, const wchar_t* a_fileName)
{
	m_spriteBatch = std::make_unique<SpriteBatch>(a_deviceContext);
	m_spriteFont = std::make_unique<SpriteFont>(a_device, a_fileName);
	DirectX::XMStoreFloat2(&m_fontSize, m_spriteFont->MeasureString(L"��"));
}

bool TextRenderer::Update(const wchar_t* a_text, int a_drawTextLen)
{
	m_textLen = std::wcslen(a_text);		//��������
	m_drawText = a_text;	//const wchar_t*��wstring�Ɋi�[����
	m_textLen = m_drawText.length(); //��������
	m_drawableTextLen = a_drawTextLen;

	if (m_textLen >= m_drawableTextLen)
	{
		return false;
	}
	return true;
}

void TextRenderer::Begin()
{
	m_spriteBatch->Begin();
}

void TextRenderer::Render(XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize, TextAlignment a_textAlignment, float a_lineSpace, FXMVECTOR a_textColor, float a_scale)
{
	std::vector<std::wstring> _textParts;	//���s���s���邲�Ƃɂ��̃R���e�i�ɒǉ����Ă���(���s�����̏ꍇ�ł��Œ�ł��v�f��1�͑��݂���݌v)

	//�������������͂���Ă�����
	if (m_textLen)
	{
		std::wstring _textPart = {};	//���s�����܂ł̕�����������ɂǂ�ǂ�i�[���Ă���
		DirectX::XMFLOAT2 _textPartSize{};	//������`�悵���ۂ̃T�C�Y���i�[����(�X�N���[�����)
		for (int i = 0; i < m_textLen; i++)
		{
			//m_endsInNewline�̏�����
			m_endIsNewline = false;
			//�R�s�[(���̕������i�[���Ă��`��\�͈͂�����Ȃ������`�F�b�N���邽�߂ɃR�s�[����)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += m_drawText.at(i);	//�p��������[i < _textLen]�ł��邽�ߖ��Ȃ��w��v�f���擾�ł���
			//������̒������擾
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//�����A�`��\�͈͂�����Ă�����
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				//��������i�[
				_textPart += m_drawText.at(i);
			}
			//���s�̓��͂�����Ă�����
			else if (m_drawText.at(i) == L'\n')
			{
				//���s����(\n)�͊i�[�������Ă悢�̂ŁA_textPart += str.at(i)���s��Ȃ�
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//��������i�[
				_textPart += m_drawText.at(i);
			}
			if (a_drawStartPosition.y - a_drawableAreaLeftTopPosition.y + (_textParts.size() + 1) * (m_fontSize.y * a_scale) + a_lineSpace * _textParts.size() > a_drawableAreaSize.y)
				break;
			if (m_textLen == i + 1)
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
	case TextRenderer::TextAlignment::Center://��������
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://�E�[����
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
			case TextRenderer::TextAlignment::Leading://���[����
				break;
			case TextRenderer::TextAlignment::Center://��������
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://�E�[����
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
			case TextRenderer::TextAlignment::Leading://���[����
				_nextDrawPosition.x += DirectX::XMVectorGetX(m_spriteFont->MeasureString(_textParts.at(_textParts.size() - 1).c_str())) * a_scale;
				break;
			case TextRenderer::TextAlignment::Center://��������
				_nextDrawPosition.x += (DirectX::XMVectorGetX(m_spriteFont->MeasureString(_textParts.at(_textParts.size() - 1).c_str())) * 0.5f) * a_scale;
				break;
			default:
				break;
			}
			_nextDrawPosition.y = a_drawStartPosition.y + (_textParts.size() - 1) * (m_fontSize.y * a_scale + a_lineSpace);
		}
	}
}

void TextRenderer::ToolRender(DirectX::XMFLOAT2 a_drawStartPosition, XMFLOAT2 a_drawableAreaLeftTopPosition, XMFLOAT2 a_drawableAreaSize,
	TextAlignment a_textAlignment, float a_lineSpace, float a_rotation, DirectX::FXMVECTOR a_textColor, float a_scale, bool a_fullScreenTestFlag)
{
	std::vector<std::wstring> _textParts;	//���s���s���邲�Ƃɂ��̃R���e�i�ɒǉ����Ă���(���s�����̏ꍇ�ł��Œ�ł��v�f��1�͑��݂���݌v)

	int _drawTextLen = m_textLen <= m_drawableTextLen ? m_textLen : m_drawableTextLen;

	//�������������͂���Ă�����
	if (_drawTextLen)
	{
		std::wstring _textPart = {};	//���s�����܂ł̕�����������ɂǂ�ǂ�i�[���Ă���
		DirectX::XMFLOAT2 _textPartSize{};	//������`�悵���ۂ̃T�C�Y���i�[����(�X�N���[�����)
		for (int i = 0; i < _drawTextLen; i++)
		{
			//m_endsInNewline�̏�����
			m_endIsNewline = false;
			//�R�s�[(���̕������i�[���Ă��`��\�͈͂�����Ȃ������`�F�b�N���邽�߂ɃR�s�[����)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += m_drawText.at(i);	//�p��������[i < _textLen]�ł��邽�ߖ��Ȃ��w��v�f���擾�ł���
			//������̒������擾
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//�����A�`��\�͈͂�����Ă�����
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				//��������i�[
				_textPart += m_drawText.at(i);
			}
			//���s�̓��͂�����Ă�����
			else if (m_drawText.at(i) == L'\n')
			{
				//���s����(\n)�͊i�[�������Ă悢�̂ŁA_textPart += str.at(i)���s��Ȃ�
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//��������i�[
				_textPart += m_drawText.at(i);
			}
			if (a_drawStartPosition.y - a_drawableAreaLeftTopPosition.y + (_textParts.size() + 1) * (m_fontSize.y * a_scale) + a_lineSpace * _textParts.size() > a_drawableAreaSize.y)
				break;
			if (_drawTextLen == i + 1)
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
	case TextRenderer::TextAlignment::Center://��������
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://�E�[����
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
			case TextRenderer::TextAlignment::Leading://���[����
				break;
			case TextRenderer::TextAlignment::Center://��������
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://�E�[����
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
	int _textLen = a_text.length();		//��������
	std::wstring str(a_text);	//const wchar_t*��wstring�Ɋi�[����
	std::vector<std::wstring> _textParts;	//���s���s���邲�Ƃɂ��̃R���e�i�ɒǉ����Ă���(���s�����̏ꍇ�ł��Œ�ł��v�f��1�͑��݂���݌v)

	//�������������͂���Ă�����
	if (_textLen)
	{
		std::wstring _textPart = {};	//���s�����܂ł̕�����������ɂǂ�ǂ�i�[���Ă���
		DirectX::XMFLOAT2 _textPartSize{};	//������`�悵���ۂ̃T�C�Y���i�[����(�X�N���[�����)
		for (int i = 0; i < _textLen; i++)
		{
			//m_endsInNewline�̏�����
			m_endIsNewline = false;
			//�R�s�[(���̕������i�[���Ă��`��\�͈͂�����Ȃ������`�F�b�N���邽�߂ɃR�s�[����)
			std::wstring _bummyTextPart = _textPart;
			_bummyTextPart += str.at(i);	//�p��������[i < _textLen]�ł��邽�ߖ��Ȃ��w��v�f���擾�ł���
			//������̒������擾
			DirectX::XMStoreFloat2(&_textPartSize, m_spriteFont->MeasureString(_bummyTextPart.c_str()));
			//�����A�`��\�͈͂�����Ă�����
			if (_textPartSize.x * a_scale >= a_drawableAreaSize.x)
			{
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				//��������i�[
				_textPart += str.at(i);
			}
			//���s�̓��͂�����Ă�����
			else if (str.at(i) == L'\n')
			{
				//���s����(\n)�͊i�[�������Ă悢�̂ŁA_textPart += str.at(i)���s��Ȃ�
				//�R���e�i�ɉ��s�����܂ł̕�����f�[�^���i�[
				_textParts.emplace_back(_textPart);
				//�N���A
				_textPart = {};
				m_endIsNewline = true;
			}
			else
			{
				//��������i�[
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
	case TextRenderer::TextAlignment::Center://��������
		_forecastPointOffset = (m_fontSize.y * 0.5f * a_scale);
		break;
	case TextRenderer::TextAlignment::Trailing://�E�[����
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
			case TextRenderer::TextAlignment::Leading://���[����
				break;
			case TextRenderer::TextAlignment::Center://��������
				_origin.x = _textPartSize.x / 2.0f;
				break;
			case TextRenderer::TextAlignment::Trailing://�E�[����
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