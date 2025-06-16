#include "SceneManager.h"
#include <memory>
#include "Input.h"
#include "Graphics.h"

SceneManager::SceneManager()
	:m_allFacialSet()
{
}

//�X�V����
void SceneManager::Update(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	Keyboard& _keyboard = Input::Instance().GeKeyboard();

	if (next_scene != nullptr)
	{
		//�Â��V�[�����I������
		Clear();

		//�V�����V�[����ݒ�
		current_scene = next_scene;
		next_scene = nullptr;


		//�V�[������������
		if (!current_scene->IsReady())
		{
			current_scene->Initialize();
		}
	}

	if (current_scene != nullptr)
	{
		if (current_scene->IsReady())
		{
			if (_keyboard.GetKeyInput(Keyboard::F1, Keyboard::DownMoment))
			{
				_graphics.ChangeFullScreen(_graphics.GetHWND(), !_graphics.IsFullscreen());
			}
		}
		current_scene->Update(elapsedTime);
	}
}

//�`�揈��
void SceneManager::Render(float elapsedTime)
{
	if (current_scene != nullptr)
	{
		current_scene->Render(elapsedTime);
	}
}

#ifdef USE_IMGUI
void SceneManager::ImGuiRender()
{
	if (current_scene != nullptr)
	{
		//current_scene->CommonImGuiRender();
		current_scene->ImGuiRender();
	}
}
#endif // USE_IMGUI

//�V�[���N���A
void SceneManager::Clear()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
	//�V�����V�[����ݒ�
	next_scene = scene;
}

//�I�u�W�F�N�g�č\�z
void SceneManager::OnSizeChange()
{
	current_scene->CommonOnSizeChange();
	current_scene->OnSizeChange();
}
