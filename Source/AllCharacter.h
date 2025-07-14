//#pragma once
//#include "Character.h"
//#include "Singleton.h"
//
//class AllCharacter : public Singleton<AllCharacter>
//{
//public:
//	AllCharacter() :Singleton() {}
//	~AllCharacter()override {}
//
//	bool InitializeElements()override;
//
//	std::vector <std::shared_ptr<Character>> GetAllCharacter() { return allCharacter; }
//private:
//	std::vector <std::shared_ptr<Character>> allCharacter;
//};