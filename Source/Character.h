#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include "Json.h"
#include "CharacterAction.h"
#include "FacialSet.h"

class CharacterAction;

class Character : public std::enable_shared_from_this<Character>
{
public:
	Character() = default;
	Character(std::string a_name, std::shared_ptr<FacialSet> a_facialSet);
	~Character() {}

	void Initialize(int a_vectorSize, int a_facialIndex = -1);
	void Update(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize);
	void CharacterEditUpdate(DirectX::XMFLOAT2 a_basePosition, DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize);
	void Render(BasePoint a_basePoint, DirectX::XMFLOAT4 a_color = { 1.0f,1.0f,1.0f,1.0f });
	void ToolOutLineRender(BasePoint a_basePoint, DirectX::XMFLOAT4 a_color = { 1.0f,1.0f,1.0f,1.0f });
	void AddAction(int a_actionIndex);
	void ActionsEnter();
	bool ActionsExecute(float a_elapsedTime);
	void ActionsExit();
	void StartSlideshow();
	void EndSlideShow();

	std::shared_ptr<Sprite> GetFacial(int a_keyIndex);
	std::shared_ptr<Sprite> GetFacial(std::string a_key);
	std::vector<std::string> GetFacialKeys();
	std::string GetFacialKey(int a_facialIndex);
	int GetKeyIndex();

	template<class T>
	void serialize(T& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(isMirror),
			CEREAL_NVP(facialStr),
			CEREAL_NVP(normalizePosition),
			CEREAL_NVP(normalizeSize),
			CEREAL_NVP(m_actions),
			CEREAL_NVP(basePositionY),
			CEREAL_NVP(baseSizeY),
			CEREAL_NVP(imageColor)
		);
	}

	std::string name = {};						//ÉLÉÉÉâÇÃñºëOÅ@ÅöÅöÅö
	std::shared_ptr<FacialSet> facialSet;
	std::string facialStr = {};
	int facialIndex = 0;

	bool isMirror = false;
	DirectX::XMFLOAT2 normalizePosition = { 0.5f,0.5f };
	DirectX::XMFLOAT2 position = {};
	DirectX::XMFLOAT2 normalizeSize = { 0.3f,0.3f };
	DirectX::XMFLOAT2 size = {};
	float basePositionY = 0.5f;
	float baseSizeY = 0.3f;
	DirectX::XMFLOAT4 imageColor = { 1.0f,1.0f,1.0f,1.0f };
	std::vector<std::shared_ptr<CharacterAction>> m_actions;
};