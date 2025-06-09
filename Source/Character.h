#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include "Json.h"

#include "FacialSet.h"

class Character
{
public:
	Character() = default;
	Character(std::string a_name, std::shared_ptr<FacialSet> a_facialSet);
	~Character() {}

	void Initialize(int a_vectorSize, int a_facialIndex = -1);
	void ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize);
	void ToolRender(int a_facialIndex, BasePoint a_basePoint, DirectX::XMFLOAT4 a_color = { 1.0f,1.0f,1.0f,1.0f });

	std::string GetName() { return name; }
	std::shared_ptr<FacialSet> GetFacialSet() { return facialSet; }
	std::shared_ptr<Sprite> GetFacial(int a_keyIndex);
	std::shared_ptr<Sprite> GetFacial(std::string a_key);
	std::vector<std::string> GetFacialKeys();

	template<class T>
	void serialize(T& archive)
	{
		archive(
			CEREAL_NVP(name),
			CEREAL_NVP(facialIndex),
			CEREAL_NVP(normalizePosition),
			CEREAL_NVP(normalizeSize)
		);
	}

public:
	std::string name = {};						//ÉLÉÉÉâÇÃñºëOÅ@ÅöÅöÅö
	std::shared_ptr<FacialSet> facialSet;
	int facialIndex = 0;

	DirectX::XMFLOAT2 normalizePosition = { 0.5f,0.5f };
	DirectX::XMFLOAT2 position = {};
	DirectX::XMFLOAT2 normalizeSize = { 0.3f,0.3f };
	DirectX::XMFLOAT2 size = {};
};