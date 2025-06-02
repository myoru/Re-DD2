#pragma once
#include "Scene.h"

///�V�[���}�l�[�W���[
class SceneManager
{
private:
	SceneManager();
	~SceneManager() {}

public:
	//�B��̃C���X�^���X���擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(float elapsedTime);

#ifdef USE_IMGUI
	//ImGui�`��
	void ImGuiRender();
#endif // USE_IMGUI

	//�V�[���N���A
	void Clear();

	//�V�[���؂�ւ�
	void ChangeScene(Scene* scene);

	void OnSizeChange();

public:
	Scene* current_scene = nullptr;
	Scene* next_scene = nullptr;
};

