#pragma once
#include "FacialSet.h"

class AllFacialSet
{
public:
	AllFacialSet();
	~AllFacialSet(){}

	static AllFacialSet& Instance() { return *instance; }

	std::vector <std::pair<std::string, std::shared_ptr<FacialSet>>> GetAllFacialSet() { return allFacialSet; }

	std::shared_ptr<FacialSet> GetFacialSet(std::string a_key);
	std::shared_ptr<FacialSet> GetFacialSet(int a_keyIndex);
	std::vector<std::string> GetKeys();
private:
	static AllFacialSet* instance;
	std::vector <std::pair<std::string, std::shared_ptr<FacialSet>>> allFacialSet;
};