#include "Graphics.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneLoading.h"

using namespace DirectX;
template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

//������
void SceneLoading::Initialize()
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();

	//Scene���ʃC���f�b�N�X�ݒ�
	Scene::SetIndex(sc_i(Scene::Index::Loading));

	CommonInitialize();

	m_spriteBox.emplace_back(std::make_unique<Sprite>(nullptr, BasePoint::LeftTop));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\HackLogo2.png"));

	//�X���b�h�̊J�n
	m_thread = new std::thread(LoadingThread, this);
}

//�I����
void SceneLoading::Finalize()
{
	//�X���b�h�I����
	if (m_thread != nullptr)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
	if (m_logoSprAlphaTrg)
	{
		m_logoSprAlpha += elapsedTime * 1.2f;
		if (m_logoSprAlpha >= 1.3f)
		{
			m_logoSprAlpha = 1.0f;
			m_logoSprAlphaTrg = false;
		}
	}
	else
	{
		m_logoSprAlpha -= elapsedTime * 1.5f;
		if (m_logoSprAlpha <= 0.0f)
		{
			m_logoSprAlpha = 0.0f;
			m_logoSprAlphaTrg = true;
		}
	}

	//���̃V�[���̏���������������V�[����؂�ւ���
	if (m_nextScene->IsReady())
		SceneManager::GetInstance()->ChangeScene(m_nextScene);
}

//�`�揈��
void SceneLoading::Render(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	RenderingState* _rendering_state = _graphics.GetRenderingState();

	//RenderTarget��Clear
	ClearRenderTarget();

	//2D�X�v���C�g�`��
	{
		_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
		_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
		_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);

		float _logoSize = screenSize.y / 8.0f;
		m_spriteBox[sc_i(SpriteKind::Logo)]->Render(BasePoint::RightBottom, screenSize.x, screenSize.y, { _logoSize ,_logoSize }, 0.0f, { 0.8666666f,0.8666666f,0.8666666f,m_logoSprAlpha > 1.0f ? 1.0f : m_logoSprAlpha });
	}
}

#ifdef USE_IMGUI
void SceneLoading::ImGuiRender()
{
}
#endif // USE_IMGUI

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	SceneManager::GetInstance()->Initialize();

	//���̃V�[���̏��������s��
	scene->m_nextScene->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->m_nextScene->SetReady();
}