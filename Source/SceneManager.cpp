#include "SceneManager.h"
#include <memory>
#include "Input.h"
#include "Graphics.h"

SceneManager::SceneManager()
	:m_allFacialSet()
{
}

//更新処理
void SceneManager::Update(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	Keyboard& _keyboard = Input::Instance().GeKeyboard();

	if (next_scene != nullptr)
	{
		//古いシーンを終了処理
		Clear();

		//新しいシーンを設定
		current_scene = next_scene;
		next_scene = nullptr;


		//シーン初期化処理
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

//描画処理
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

//シーンクリア
void SceneManager::Clear()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

//シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
	//新しいシーンを設定
	next_scene = scene;
}

//オブジェクト再構築
void SceneManager::OnSizeChange()
{
	current_scene->CommonOnSizeChange();
	current_scene->OnSizeChange();
}
