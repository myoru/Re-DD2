#pragma once

#include <memory>
#include <string>
#include <map>

//リソースマネージャー
class ResourceManager
{
private:
	ResourceManager() {}
	~ResourceManager() {}

public:
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}
private:
};