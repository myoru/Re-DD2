#include "Character.h"
#include "CharacterActions.h"

Character::Character(std::string a_name, std::shared_ptr<FacialSet> a_facialSet)
	:name(a_name), facialSet(a_facialSet)
{
	//facialStr = facialSet->GetKey(facialIndex);
}

void Character::Initialize(int a_vectorSize, int a_facialIndex)
{
	if (a_facialIndex >= 0)
	{

	}
}

void Character::Update(float a_elapsedTime, DirectX::XMFLOAT2 a_reviewLeftTop, DirectX::XMFLOAT2 a_reviewSize)
{
	position.x = a_reviewLeftTop.x + a_reviewSize.x * normalizePosition.x;
	position.y = a_reviewLeftTop.y + a_reviewSize.y * normalizePosition.y;
	size.y = a_reviewSize.y * normalizeSize.y;
	size.x = size.y / facialSet->GetFacial(0)->GetAspectRation();
}

void Character::Render(BasePoint a_basePoint, DirectX::XMFLOAT4 a_color)
{
	facialSet->GetFacial(facialIndex)->Render(a_basePoint, position, size, 0.0f, a_color);
}

void Character::ToolOutLineRender(BasePoint a_basePoint, DirectX::XMFLOAT4 a_color)
{
	facialSet->GetFacial(facialIndex)->OutLineRender(a_basePoint, position.x, position.y, size.x * 1.05f, size.y * 1.05f, 0.0f, a_color);
}

void Character::AddAction(int a_actionIndex)
{
	switch (a_actionIndex)
	{
	case 0:
		m_actions.emplace_back(std::make_shared<Move>(CharacterAction::Data{ shared_from_this(),"Move",0,0.0f,1.0f }));
		break;
	case 1:
		m_actions.emplace_back(std::make_shared<SEStart>(CharacterAction::Data{ shared_from_this(),"SEStart",0 }));
		break;
	case 2:
		m_actions.emplace_back(std::make_shared<Shake>(CharacterAction::Data{ shared_from_this(),"Shake",0 }));
		break;
	}
}

void Character::ActionsEnter()
{
	for (auto _characterAction : m_actions)
	{
		_characterAction->Enter();
	}
}

bool Character::ActionsExecute(float a_elapsedTime)
{
	bool _endCheck = true;

	for (auto _characterAction : m_actions)
	{
		if (!_characterAction->Execute(a_elapsedTime))
		{
			_endCheck = false;
		}
	}

	return _endCheck;
}

void Character::ActionsExit()
{
	for (auto _characterAction : m_actions)
	{
		_characterAction->Exit();
	}
}

void Character::StartSlideshow()
{
	for (auto& _action : m_actions)
	{
		_action->Initialize();
	}
}

void Character::EndSlideShow()
{
	for (auto& _action : m_actions)
	{
		_action->Finalize();
	}
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

std::string Character::GetFacialKey(int a_facialIndex)
{
	return GetFacialKeys()[a_facialIndex];
}

int Character::GetKeyIndex()
{
	int _index = 0;
	for (auto _key : facialSet->keys)
	{
		if (_key == facialStr)
		{
			return _index;
		}
		_index++;
	}
	return -1;
}
