#pragma once
#include <d3d11.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "TextRenderer.h"
#include "Sprite.h"
#include "Json.h"

using namespace DirectX;

class SignBoard
{
public:
	SignBoard(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, std::string a_jsonFileName);
	~SignBoard();

	void Update(float a_elapsedTime, DirectX::XMFLOAT2 a_screenLeftTopPosition = { 0.0f,0.0f }, DirectX::XMFLOAT2 a_screenSize = { -1.0f,-1.0f });
	void BoardRender();
	void TextRender(FXMVECTOR a_textColor = Colors::Red);
	void ToolRender(bool a_fullscreen,FXMVECTOR a_textColor = Colors::White);

	Sprite* GetBoardSprite() { return m_boardSpr.get(); }
	
#ifdef USE_IMGUI
	
#endif
	//Json��p���ē��o�͂���f�[�^�A�܂��͕K�v�ȃf�[�^
	struct JsonElements
	{
		std::string boardFilePath{};							//�{�[�h�Ɏg���摜�̃p�X(����)
		std::string fontFilePath{};								//�e�L�X�g�Ɏg���t�H���g�̃p�X(����)
		char jsonFileName[256]{};								//�o�͂���Json�t�@�C���̖��O

		std::string text{};										//�`�悷�镶����
		DirectX::XMFLOAT2 boardPosition{};						//�X�N���[�����
		DirectX::XMFLOAT2 boardLocalPosition{ 0.5f,0.5f };		//�X�N���[���T�C�Y�ɑ΂��ĉ����̈ʒu�ł��邩(X������0.7f�Ȃ牡���̈ʒu�͉�ʂ̉�����7:3�ŕ�������ʒu)
		DirectX::XMFLOAT2 boardSize{};							//�X�N���[�����
		DirectX::XMFLOAT2 boardLocalSize{0.9f,0.0f};			//�X�N���[���T�C�Y�ɑ΂��ĉ����̑傫���ł��邩(X������0.7f�Ȃ牡���͉�ʂ̉����̑傫����0.7f�������傫��)

		int textAlignment = static_cast<int>(TextRenderer::TextAlignment::Center);	//�A���C�����g(���[�����A���������A�E�[����)
		DirectX::XMFLOAT2 textDrawableAreaPosition{};								//�X�N���[�����
		DirectX::XMFLOAT2 textDrawableAreaLocalPosition{ 0.5f,0.5f };				//�{�[�h�T�C�Y�ɑ΂��ĉ����̈ʒu�ł��邩(X������0.7f�Ȃ牡���̈ʒu�̓{�[�h�̉�����7:3�ŕ�������ʒu)
		DirectX::XMFLOAT2 textDrawableAreaSize{};									//�X�N���[�����
		DirectX::XMFLOAT2 textDrawableAreaLocalSize{0.9f,0.9f};						//�{�[�h�T�C�Y�ɑ΂��ĉ����̑傫���ł��邩(X������0.7f�Ȃ牡���̓{�[�h�̉����̑傫����0.7f�������傫��)

		DirectX::XMFLOAT2 textDrawStartPosition = {};						//�X�N���[�����
		DirectX::XMFLOAT2 textDrawStartLocalPosition = { 0.5f,0.5f };		//�e�L�X�g�`��\�G���A�̃T�C�Y�ɑ΂��ĉ����̈ʒu�ł��邩(X������0.7f�Ȃ牡���̈ʒu�̓e�L�X�g�`��\�G���A�̉�����7:3�ŕ�������ʒu)
		float textScale{};													//�e�L�X�g�`�掞�Ɏg��
		float textLocalHeight{0.3f};										//�e�L�X�g�`��\�G���A�̃T�C�Y�ɑ΂��ĉ����̑傫���ł��邩(0.7f�Ȃ�e�L�X�g�`��\�G���A�̏c���̑傫����0.7f�������傫��)
		float localLineSpace = 0.0f;										//�s�Ԋu(���[�J��)
		float lineSpace = 0.0f;												//�s�Ԋu
		bool autoAspectChange = true;										//�{�[�h�̃A�X�y�N�g����Œ肷�邩�̃t���O

		template<class T>
		void serialize(T& archive)
		{
			archive(
				CEREAL_NVP(boardFilePath),
				CEREAL_NVP(fontFilePath),
				CEREAL_NVP(boardLocalPosition),
				CEREAL_NVP(boardLocalSize),
				CEREAL_NVP(textAlignment),
				CEREAL_NVP(textDrawableAreaLocalPosition),
				CEREAL_NVP(textDrawableAreaLocalSize),
				CEREAL_NVP(textDrawStartLocalPosition),
				CEREAL_NVP(textLocalHeight),
				CEREAL_NVP(localLineSpace),
				CEREAL_NVP(autoAspectChange));
		}
	};
	JsonElements jElements;
private:
	std::unique_ptr<Sprite> m_whiteSpr;
	std::unique_ptr<Sprite> m_boardSpr;
	std::unique_ptr<TextRenderer> m_textRenderer;
public:
	char m_inputBuffer[256] = {};
};
