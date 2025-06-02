//#include "AllCharacters.h"
//#include "TestCharacter.h"
//#include "TestCharacter2.h"
//
//AllCharacters* AllCharacters::instance = nullptr;
//
//AllCharacters::AllCharacters()
//{
//	/*std::vector<const wchar_t*> _spritePaths =
//	{
//		L".\\Data\\Sprite\\AtariBattleIcon.png",
//	};*/
//	allCharacters.emplace_back("TestCharacter",std::make_shared<TestCharacter>("TestCharacter"));
//	allCharacters.emplace_back("TestCharacter2",std::make_shared<TestCharacter2>("TestCharacter2"));
//	/*_spritePaths = {};
//	_spritePaths =
//	{
//		L".\\Data\\Sprite\\HazureBattleIcon.png",
//	};
//	allCharacters["Hazure"] = std::make_shared<Character>("Hazure", _spritePaths);*/
//	instance = this;
//}
//
//std::shared_ptr<Character> AllCharacters::GetCharacter(std::string a_key)
//{
//	std::vector<std::string> _keys = GetAllCharactersKeys();
//	const std::string& targetKey = a_key;
//
//	for (const auto& [key, sprite] : allCharacters)
//	{
//		if (key == targetKey)
//		{
//			return sprite;
//		}
//	}
//	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
//}
//
//std::shared_ptr<Character> AllCharacters::GetCharacter(int a_keyIndex)
//{
//	std::vector<std::string> _keys = GetAllCharactersKeys();
//	const std::string& targetKey = _keys[a_keyIndex];
//
//	for (const auto& [key, character] : allCharacters)
//	{
//		if (key == targetKey)
//		{
//			return character;
//		}
//	}
//	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
//}
//
//std::vector<std::string> AllCharacters::GetAllCharactersKeys()
//{
//	std::vector<std::string> _keys;
//	_keys.reserve(allCharacters.size());
//	for (const auto& [_key, _] : allCharacters)
//	{
//		_keys.push_back(_key);
//	}
//	return _keys;
//}
