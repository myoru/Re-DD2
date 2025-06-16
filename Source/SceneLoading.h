#pragma once

#include "Scene.h"
#include "Sprite.h"
#include <thread>

//ローディングシーン
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) :m_nextScene(nextScene){}
	~SceneLoading()override {}

	//初期化
	void Initialize() override;

	//終了化
	void Finalize()override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render(float elapsedTime)override;

#ifdef USE_IMGUI
	//ImGui描画関数
	void ImGuiRender()override;
#endif // USE_IMGUI

private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:
	float m_logoSprAlpha = 0.0f;
	bool m_logoSprAlphaTrg = true;

	Scene* m_nextScene = nullptr;
	std::thread* m_thread = nullptr;

	enum class SpriteKind
	{
		White,
		Logo,
	};
};
