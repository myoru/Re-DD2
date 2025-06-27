#pragma once
#include "Slide.h"
#include "Json.h"
#include <map>
#include "SignBoard.h"

class Chapter
{
public:
	Chapter()
	{
		m_slides.emplace_back(Slide());
		m_currentSlide = &m_slides[m_currentSlideIndex];
	}
	~Chapter() {}

	void Update(float a_elapsedTime);
	void ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize);
	void Render();
	void ToolRender(DirectX::XMFLOAT2 a_reviewScreenLeftTopPos, DirectX::XMFLOAT2 a_reviewScreenSize, bool a_isFullReview);

	std::vector<Slide>& GetSlides() { return m_slides; }

	template<class T>
	void serialize(T& archive)
	{
		archive(CEREAL_NVP(m_slides));
	}

	static void LoadSlideValue(const char* filename, Chapter& chapter)
	{
		std::ifstream ifs(filename);
		cereal::JSONInputArchive archive(ifs);
		archive(CEREAL_NVP(chapter));
	}

	static void SaveSlideValue(const char* filename, Chapter& chapter)
	{
		std::ofstream ofs(filename);
		cereal::JSONOutputArchive archive(ofs);
		archive(CEREAL_NVP(chapter));
	}

protected:
	std::vector<Slide> m_slides;
public:
	Slide* m_currentSlide = nullptr; //���݂̃X���C�h
	int m_currentSlideIndex = 0; //�����ڂ̃X���C�h��I�����Ă��邩�̕ϐ�(0�Ȃ�1����)
	std::vector<std::unique_ptr<SignBoard>> m_signBoards; //�e�L�X�g�E�B���h�E�p�|�C���^�ϐ�
	//float m_textTimer = 0.0f; //�e�L�X�g����Ɏg���^�C�}�[(int�^��cast���Ďg��)
	bool m_textReadEndFlag = false;
};