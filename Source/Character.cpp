#include "Character.h"

Character::Character(std::string a_name, std::shared_ptr<FacialSet> a_facialSet)
	:name(a_name),facialSet(a_facialSet)
{
}

void Character::ToolUpdate(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize)
{
	position.x = a_reviewLeftTop.x + a_reviewSize.x * normalizeSize.x;
	position.y = a_reviewLeftTop.y + a_reviewSize.y * normalizeSize.y;

	/*size.x = a_reviewLeftTop.x + a_reviewSize.x * normalizeSize.x;
	size.y = a_reviewLeftTop.y + a_reviewSize.y * normalizeSize.y;*/
}

std::shared_ptr<Sprite> Character::GetFacial(int a_keyIndex)
{
	std::vector<std::string> _keys = GetFacialKeys();
	const std::string& targetKey = _keys[a_keyIndex];

	for (const auto& [key, sprite] : facialSet->data)
	{
		if (key == targetKey)
		{
			return sprite;
		}
	}
	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
}

std::shared_ptr<Sprite> Character::GetFacial(std::string a_key)
{
	std::vector<std::string> _keys = GetFacialKeys();
	const std::string& targetKey = a_key;

	for (const auto& [key, sprite] : facialSet->data)
	{
		if (key == targetKey)
		{
			return sprite;
		}
	}
	return nullptr; // Œ©‚Â‚©‚ç‚È‚¯‚ê‚Î null
}

std::vector<std::string> Character::GetFacialKeys()
{
	std::vector<std::string> keys;
	keys.reserve(facialSet->data.size());
	for (const auto& [key, _] : facialSet->data)
	{
		keys.push_back(key);
	}
	return keys;
}
