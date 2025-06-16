#pragma once

#include "Scene.h"
#include "Sprite.h"
#include <thread>

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) :m_nextScene(nextScene){}
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
