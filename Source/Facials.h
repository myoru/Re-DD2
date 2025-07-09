#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include "Sprite.h"

class Facials
{
public:
	Facials(){}
	virtual ~Facials(){}
private:
	std::vector<std::string> m_facialKeys;
	std::unordered_map<std::string, std::shared_ptr<Sprite>> m_facials;
};