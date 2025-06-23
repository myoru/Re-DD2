#pragma once
#include "Scene.h"
#include "Singleton.h"
#include "AllFacialSet.h"
#include "AllBGM.h"

///シーンマネージャー
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager() :Singleton() {}
	~SceneManager()override {}

private:
	bool InitializeElements()override;
public:
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
	//画面サイズ切り替え時に呼ばれる関数
	void OnSizeChange();

public:
	Scene* current_scene = nullptr;
	Scene* next_scene = nullptr;

private:
	AllFacialSet m_allFacialSet;
	AllBGM m_allBGM;
};

