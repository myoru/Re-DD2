#pragma once
#include "Scene.h"

///シーンマネージャー
class SceneManager
{
private:
	SceneManager();
	~SceneManager() {}

public:
	//唯一のインスタンスを取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(float elapsedTime);

#ifdef USE_IMGUI
	//ImGui描画
	void ImGuiRender();
#endif // USE_IMGUI

	//シーンクリア
	void Clear();

	//シーン切り替え
	void ChangeScene(Scene* scene);

	void OnSizeChange();

public:
	Scene* current_scene = nullptr;
	Scene* next_scene = nullptr;
};

