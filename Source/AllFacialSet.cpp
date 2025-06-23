#include "AllFacialSet.h"
#include "TestFacials1.h"
#include "TestFacials2.h"
#include "ReimuFacials.h"
#include "MarisaFacials.h"

bool AllFacialSet::InitializeElements()
{
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"—EŒ³—²u"), std::make_shared<TestFacials1>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"—EŒ³‘å‹P"), std::make_shared<TestFacials2>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"‚ä‚Á‚­‚è—ì–²"), std::make_shared<ReimuFacials>());
	allFacialSet.emplace_back(reinterpret_cast<const char*>(u8"‚ä‚Á‚­‚è–‚—¹"), std::make_shared<MarisaFacials>());

	return true;
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
	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
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
	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
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
