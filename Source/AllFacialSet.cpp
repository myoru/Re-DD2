#include "AllFacialSet.h"
#include "TestFacials1.h"
#include "TestFacials2.h"
#include "ReimuFacials.h"
#include "MarisaFacials.h"

AllFacialSet* AllFacialSet::instance = nullptr;

AllFacialSet::AllFacialSet()
{
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"óEå≥ó≤éu"), std::make_shared<TestFacials1>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"óEå≥ëÂãP"), std::make_shared<TestFacials2>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"Ç‰Ç¡Ç≠ÇËóÏñ≤"), std::make_shared<ReimuFacials>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"Ç‰Ç¡Ç≠ÇËñÇóùçπ"), std::make_shared<MarisaFacials>());

	/*allFacialSet.emplace_back("Test1", std::make_shared<TestFacials1>());
	allFacialSet.emplace_back("Test2", std::make_shared<TestFacials2>());
	allFacialSet.emplace_back("Reimu", std::make_shared<ReimuFacials>());
	allFacialSet.emplace_back("Marisa", std::make_shared<MarisaFacials>());*/

	instance = this;
}

std::shared_ptr<FacialSet> AllFacialSet::GetFacialSet(std::string a_targetKey)
{
	std::vector<std::string> _keys = GetKeys();

	for (const auto& [_key, _sprite] : allFacialSet)
	{
		if (_key == a_targetKey)
		{
			return _sprite;
		}
	}
	return nullptr; // å©Ç¬Ç©ÇÁÇ»ÇØÇÍÇŒ null
}

std::shared_ptr<FacialSet> AllFacialSet::GetFacialSet(int a_targetKeyIndex)
{
	std::vector<std::string> _keys = GetKeys();
	const std::string& _targetKey = _keys[a_targetKeyIndex];

	for (const auto& [key, character] : allFacialSet)
	{
		if (key == _targetKey)
		{
			return character;
		}
	}
	return nullptr; // å©Ç¬Ç©ÇÁÇ»ÇØÇÍÇŒ null
}

std::vector<std::string> AllFacialSet::GetKeys()
{
	std::vector<std::string> _keys;
	_keys.reserve(allFacialSet.size());
	for (const auto& [_key, _] : allFacialSet)
	{
		_keys.push_back(_key);
	}
	return _keys;
}
