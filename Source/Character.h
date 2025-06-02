#pragma once
#include <sstream>
#include <iostream>
#include <fstream>
#include "Json.h"

#include "FacialSet.h"

class Character
{
public:
	Character(std::string a_name,std::shared_ptr<FacialSet> a_facialSet);
	~Character() {}

	void ToolUpdate(float a_elapsedTime,DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize);

	std::string GetName() { return name; }
	std::shared_ptr<FacialSet> GetFacialSet() { return facialSet; }
	std::shared_ptr<Sprite> GetFacial(int a_keyIndex);
	std::shared_ptr<Sprite> GetFacial(std::string a_key);
	std::vector<std::string> GetFacialKeys();
	
public:
	std::string name = {};						//ÉLÉÉÉâÇÃñºëOÅ@ÅöÅöÅö
	std::shared_ptr<FacialSet> facialSet;
	int facialIndex = 0;

	DirectX::XMFLOAT2 normalizePosition = { 0.5f,0.5f };
	DirectX::XMFLOAT2 position = {};
	DirectX::XMFLOAT2 normalizeSize = { 0.5f,0.5f };
	DirectX::XMFLOAT2 size = {};
	DirectX::XMFLOAT2 normalizeScale = {1.0f,1.0f};
};