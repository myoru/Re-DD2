#pragma once
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "Sprite.h"

class FacialSet
{
public:
	FacialSet(){}
	~FacialSet(){}

	void AddFacial(const std::string& name, std::shared_ptr<Sprite> sprite)
	{
		if (data.count(name) == 0) 
		{
			keys.push_back(name);
			data[name] = std::move(sprite);
		}
	}

	std::shared_ptr<Sprite> GetFacial(const std::string& a_targetKey)
	{
		auto it = data.find(a_targetKey);
		if (it != data.end()) 
		{
			return it->second;
		}
	}

	std::shared_ptr<Sprite> GetFacial(const int& a_targetKeyIndex)
	{
		const std::string _targetKey = keys[a_targetKeyIndex];
		return GetFacial(_targetKey);
	}

	std::vector<std::string> keys;
	std::unordered_map<std::string, std::shared_ptr<Sprite>> data;
};