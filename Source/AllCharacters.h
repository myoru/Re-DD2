//#pragma once
//#include "Character.h"
//#include <map>
//#include <unordered_map>
//#include <memory>
//
//#define PAD_MAX 5
//
//// �C���v�b�g
//class AllCharacters
//{
//public:
//	AllCharacters();
//	~AllCharacters() {}
//
//public:
//	// �C���X�^���X�擾
//	static AllCharacters& Instance() { return *instance; }
//	std::vector <std::pair<std::string, std::shared_ptr<Character>>>& GetAllCharacters() { return allCharacters; }
//
//	std::shared_ptr<Character> GetCharacter(std::string a_key);
//	std::shared_ptr<Character> GetCharacter(int a_keyIndex);
//	std::vector<std::string> GetAllCharactersKeys();
//private:
//	static AllCharacters* instance;
//	std::vector <std::pair<std::string, std::shared_ptr<Character>>> allCharacters;
//};
