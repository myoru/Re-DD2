#pragma once
#include <memory>
#include <iostream>
#include "Audio.h"
#include "Sprite.h"
#include "SameSprite.h"
#include "Shader.h"
#include "FullscreenQuad.h"
#include "ConstantBuffer.h"
#include "Framebuffer.h"
#include "ImGuiPack.h"

struct FadeConstants;

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	//������
	virtual void Initialize() = 0;
	//�I����
	virtual void Finalize() = 0;

#pragma region CommonScenes
	//���ʍ��ڏ�����(static�ϐ�)
	void CommonInitialize();
	void CommonOnSizeChange();
#pragma endregion CommonScenes

	//�X�V����
	virtual void Update(float elapsedTime) = 0;
	//�`�揈��
	virtual void Render(float elapsedTime) = 0;
#ifdef USE_IMGUI
	//ImGui�`��֐�
	virtual void ImGuiRender() = 0;
	virtual void CommonImGuiRender(DirectX::XMFLOAT2 a_position = { -1.0f,1.0f }, DirectX::XMFLOAT2 a_size = { -1.0f,1.0f }, ImGuiWindowFlags a_flags = ImGuiWindowFlags_None);
#endif // USE_IMGUI
	//RenderTarget��Clear�֐�
	void ClearRenderTarget();
	//��ʃT�C�Y�ύX���ɌĂ΂��֐�(���z�֐�)
	virtual void OnSizeChange() {};

#pragma region Getter&Setter
	//ready
	bool IsReady() const { return m_ready; }
	void SetReady() { m_ready = true; }
	//index
	int GetIndex() const { return m_index; }
	void SetIndex(int a_index) { m_index = a_index; }
	std::string GetIndexStr();
#pragma endregion Getter&Setter

protected:
#pragma region CommonScenes
	//�t�@�C�i���p�X�`��p�ϐ�
	static std::unique_ptr<FullscreenQuad> m_bitBlockTransfer;
	//FrameBuffer�n
	static std::unique_ptr<FrameBuffer> m_fb_fade ; //�t�F�[�h�C���E�t�F�[�h�A�E�g�p�̃t���[���o�b�t�@
	//PixelShader(�s�N�Z���V�F�[�_�[)
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_fb_ps_fade; //�t�F�[�h�C���E�t�F�[�h�A�E�g�p�̃s�N�Z���V�F�[�_�[
	//�萔�o�b�t�@
	static std::unique_ptr<ConstantBuffer<FadeConstants>> m_cb_fade; //�t�F�[�h�C���E�t�F�[�h�A�E�g�p�̒萔�o�b�t�@
	//���������������m�F����ϐ�
	static bool m_initializeChecker;
	//Clear�J���[
	static DirectX::XMFLOAT4 m_clearColor;
	//�w�i�F
	static DirectX::XMFLOAT4 m_backColor;
	//��ʃT�C�Y
	static DirectX::XMFLOAT2 screenSize;
#pragma endregion CommonScenes
	//BGM�ESE
	std::vector<std::unique_ptr<Audio>> m_seBox;
	std::vector<std::unique_ptr<Audio>> m_bgmBox;
	//Sprite(�X�v���C�g)(�e�h���N���X���Ƃ�enum class�Œ�`���� ��.SceneTitle::SpriteKind::TitkeLogo)
	std::vector<std::unique_ptr<Sprite>> m_spriteBox;
	std::vector<std::unique_ptr<SameSprite>> m_sameSpriteBox;
private:
	//Scene���ʃC���f�b�N�X
	int m_index = 0;
protected:
	//���������t���O
	bool m_ready = false;
public:
	enum class Index
	{
		Test,
		Loading,
		Title,
		Game,
		Tool,
		Event,
	};
};

//�t�F�[�h�C���E�t�F�[�h�A�E�g�p�̍\����(�萔�o�b�t�@�Ŏg��)
struct FadeConstants
{
	//����������
	void Initialize(float time, float fadeValue, int inOrOut, int isStart, int blackOrWhite = -1);
	//�X�V����
	bool Update(float elapsedTime);

	float m_time = 0.5f;       // �t�F�[�h�ɂ����鎞��
	float m_fadeValue = 0.0f; // ���݂̃t�F�[�h�l
	int m_inOutFlag = 1;       // 1: in, 0: out
	int m_isStart = 0;           // 1: �J�n, 0: ���J�n
	int m_blackWhite = 0;	 // 1: Black, 0: White

	enum class Fade
	{
		Out,
		In,
	};
	enum class BlackWhite
	{
		Clear = -1,
		Black = 0,
		White = 1,
	};
};