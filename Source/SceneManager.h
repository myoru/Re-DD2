#pragma once
#include "Scene.h"
#include "Singleton.h"
#include "AllFacialSet.h"
#include "AllBGM.h"

///�V�[���}�l�[�W���[
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager() :Singleton() {}
	~SceneManager()override {}

private:
	bool InitializeElements()override;
public:
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
	//��ʃT�C�Y�؂�ւ����ɌĂ΂��֐�
	void OnSizeChange();

public:
	Scene* current_scene = nullptr;
	Scene* next_scene = nullptr;

private:
	AllFacialSet m_allFacialSet;
	AllBGM m_allBGM;
};

