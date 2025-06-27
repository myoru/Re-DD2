#pragma once
#include "Character.h"
#include "Action.h"
#include "Input.h"

class Slide
{
public:
	enum class Type;
	Slide(Type a_type = Type::Normal);
	~Slide();

	//�L�����N�^�[�폜�֐�  ��Tool�p
	void DeleteCharacter();
	//�X�V����
	bool Update(float a_elapsedTime, const Mouse& a_mouse);
	//�`��֐�
	void Render(DirectX::XMFLOAT2 a_reviewLeftTopPos,DirectX::XMFLOAT2 a_reviewSize);
public:
	//�X���C�h�̎��
	enum class Type
	{
		Normal,
		TextDrawName,
		TextNotName,
		NotText,
	};

	enum class State
	{
		StartUp,
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
	int m_type = static_cast<int>(Type::Normal); //�X���C�h�̃^�C�v
	int m_state = static_cast<int>(State::StartUp); //�X�e�[�g
	std::vector<std::shared_ptr<Character>> m_characters; //�X���C�h�ɓo�ꂷ��L�����N�^�[
	std::set<std::shared_ptr<Character>> m_removes; //�L�����N�^�[�̍폜���X�g ��Tool�p
	int m_characterIndex = 0; //�ǂ̃L�����N�^�[��I��ł��邩 �������炭Tool�p
	std::shared_ptr<Sprite> m_backSpr; //�w�i�X�v���C�g
	std::string m_backSprFilePath{}; //�w�i�X�v���C�g�̃t�@�C���p�X  ��Json
	char m_inputBuffer[256] = {}; //�e�L�X�g�E�B���h�E�p������
	std::string m_text; //�e�L�X�g�E�B���h�E�p������  ��Json
	float m_mainTimer{}; //�X���C�h���Ƃ̃^�C�}�[
	float m_textTimer = 0.0f; //�e�L�X�g����Ɏg���^�C�}�[(int�^��cast���Ďg��)
	std::vector<std::shared_ptr<Action>> m_actions; //�X���C�h�ōs���A�N�V�������X�g(��,BGM�Đ��E��~)
	float m_startUpTimer = 0.0f;
	float m_startUpTime = 0.35f;
	float m_readEndTimer = 0.0f;
	float m_readEndTime = 0.2f;
};