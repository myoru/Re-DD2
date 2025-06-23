#pragma once
#include "FacialSet.h"
#include "Singleton.h"

class AllFacialSet : public Singleton<AllFacialSet>
{
public:
	AllFacialSet() :Singleton() {}
	~AllFacialSet()override {}

	bool InitializeElements()override;

	std::vector <std::pair<std::string, std::shared_ptr<FacialSet>>> GetAllFacialSet() { return allFacialSet; }

	std::shared_ptr<FacialSet> GetFacialSet(std::string a_key);
	std::shared_ptr<FacialSet> GetFacialSet(int a_keyIndex);
	std::vector<std::string> GetKeys();
private:
	std::vector <std::pair<std::string, std::shared_ptr<FacialSet>>> allFacialSet;
};