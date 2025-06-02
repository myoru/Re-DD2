#pragma once

#include "Scene.h"
#include "Sprite.h"
#include <thread>

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene ,bool drawExplanation = false) :next_scene(nextScene), draw_explanation(drawExplanation){}
	~SceneLoading()override {}

	//������
	void Initialize() override;

	//�I����
	void Finalize()override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render(float elapsedTime)override;

#ifdef USE_IMGUI
	//ImGui�`��֐�
	void ImGuiRender()override;
#endif // USE_IMGUI

private:
	//���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);

private:
	std::unique_ptr<SameSprite> white_spr = nullptr;
	/*std::unique_ptr<SameSprite> hack_logo_spr = nullptr;
	std::unique_ptr<SameSprite> explanation_spr = nullptr;*/
	
	bool draw_explanation;
	float hack_logo_spr_alpha = 0.0f;
	bool hack_logo_spr_alpha_trg = true;

	Scene* next_scene = nullptr;
	std::thread* thread = nullptr;
};
