#pragma once

#include <memory>
#include <string>
#include <map>

//���\�[�X�}�l�[�W���[
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