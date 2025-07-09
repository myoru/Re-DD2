#include "CharacterActions.h"

using namespace DirectX;

//*****Move*********************************************************************************//
void Move::Initialize()
{
	useImGui = true;
	m_whiteSpr = std::make_unique<Sprite>(nullptr);
}

void Move::Finalize()
{
	m_data.isEnd = false;
	m_data.timer = 0.0f;

	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return;

	_charaPtr->normalizePosition = m_startNormalizePos;
}

void Move::Enter()
{
	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return;

	_charaPtr->normalizePosition = m_startNormalizePos;
}

bool Move::Execute(float a_elapsedTime)
{
	if (m_data.isEnd) return m_data.isEnd;

	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return m_data.isEnd;

	m_data.timer += a_elapsedTime;
	float _rate = m_data.power * m_data.timer;
	if (_rate >= 1.0f)
	{
		_rate = 1.0f;
		m_data.isEnd = true;
	}
	_charaPtr->normalizePosition.x = Mathf::Lerp(m_startNormalizePos.x, m_goalNormalizePos.x, _rate);
	_charaPtr->normalizePosition.y = Mathf::Lerp(m_startNormalizePos.y, m_goalNormalizePos.y, _rate);

	return m_data.isEnd;
}

void Move::Exit()
{
	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return;

	_charaPtr->normalizePosition = m_startNormalizePos;
}

void Move::ShowExecute()
{
	std::string label = characterActionStrList[static_cast<int>(CharacterActionList::Move)] + "##MoveHeader" + std::to_string(static_cast<int>(Attribute::Execute));
	if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::RadioButton("Start", &m_data.index, static_cast<int>(Mode::Start));
		ImGui::SameLine();
		ImGui::RadioButton("Goal", &m_data.index, static_cast<int>(Mode::Goal));
	}
}

void Move::ToolUpdate(float a_elapsedTime)
{
	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return;

	m_startNormalizePos = _charaPtr->normalizePosition;
}

DirectX::XMMATRIX Move::GuizmoTranslate()
{
	DirectX::XMMATRIX _translate = {};
	_translate = DirectX::XMMatrixTranslation(m_goalNormalizePos.x, m_goalNormalizePos.y, 0.0f);
	return _translate;
}

void Move::GuizmoDecompose(DirectX::XMMATRIX a_taransform)
{
	DirectX::XMVECTOR _scale, _rotation, _translation;
	DirectX::XMMatrixDecompose(&_scale, &_rotation, &_translation, a_taransform);

	DirectX::XMFLOAT2 _tartget = {};

	DirectX::XMStoreFloat2(&_tartget, _translation);
	_tartget.x = std::clamp(_tartget.x, FLT_EPSILON, 1.0f);
	_tartget.y = std::clamp(_tartget.y, FLT_EPSILON, 1.0f);

	m_goalNormalizePos = _tartget;
}

void Move::UiRender(DirectX::XMFLOAT2 a_screenLeftTopPos, DirectX::XMFLOAT2 a_screenSize)
{
	XMFLOAT2 _goalPos = { a_screenLeftTopPos.x + a_screenSize.x * m_goalNormalizePos.x,a_screenLeftTopPos.y + a_screenSize.y * m_goalNormalizePos.y };
	XMFLOAT2 _startPos = { a_screenLeftTopPos.x + a_screenSize.x * m_startNormalizePos.x,a_screenLeftTopPos.y + a_screenSize.y * m_startNormalizePos.y };
	float _size = a_screenSize.x * 0.03f;

	//goalPosition‚ð‰ÂŽ‹‰»
	m_whiteSpr->Render(BasePoint::Center, _goalPos, { _size ,_size }, 0.0f, { 1.0f,0.0f,0.0f,0.7f });

	//“ñ“_‚Ì‚È‚·Šp‚ð‹‚ß‚é
	float _dx = _goalPos.x - _startPos.x;
	float _dy = _goalPos.y - _startPos.y;
	angle = XMConvertToDegrees(std::atan2f(_dy, _dx));

	//“ñ“_‚ðŒ‹‚Ôü‚Ì •A‘¾‚³ ‚ðŒˆ‚ß‚é(‹‚ß‚é)
	float _lineWidth = a_screenSize.x * 0.005f;
	XMVECTOR _startToGoalVec = XMVectorSubtract(XMLoadFloat2(&_goalPos), XMLoadFloat2(&_startPos));
	float _lineLen = XMVectorGetX(XMVector2Length(_startToGoalVec));
	//“ñ“_‚ðŒ‹‚Ôü‚ð•`‰æ
	m_whiteSpr->Render(BasePoint::Left, _startPos, { _lineLen,_lineWidth }, angle, { 0.0f,0.0f,0.0f,0.5f }, false);
}

//*****SEStart*********************************************************************************//
void SEStart::Enter()
{
	AllAudio::GetInstance()->GetSE(m_data.index)->Play(true);
	m_data.isEnd = true;
}

//*****Shake*********************************************************************************//
void Shake::Enter()
{
	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return;

	m_beginCharacterNormalizewPosition = _charaPtr->normalizePosition;
}

bool Shake::Execute(float a_elapsedTime)
{
	auto _charaPtr = m_data.characterPtr.lock();
	if (_charaPtr == nullptr)return m_data.isEnd;
	if (m_data.isEnd) return m_data.isEnd;

	m_data.timer -= a_elapsedTime;
	if (m_data.timer < 0.0f)
	{
		m_data.timer = 0.0f;
		m_data.isEnd = true;
	}


	switch (static_cast<Direction>(m_data.index))
	{
	case Direction::Horizontal:
		_charaPtr->normalizePosition.x =
			m_beginCharacterNormalizewPosition.x + (m_data.power * sinf(m_data.timer));
		break;
	case Direction::Vertical:
		_charaPtr->normalizePosition.y =
			m_beginCharacterNormalizewPosition.y + (m_data.power * sinf(m_data.timer));
		break;
	case Direction::Random:
		_charaPtr->normalizePosition.x =
			m_beginCharacterNormalizewPosition.x + (m_data.power * Mathf::RandomRange(-1.0f, 1.0f) * sinf(m_data.timer));
		_charaPtr->normalizePosition.y =
			m_beginCharacterNormalizewPosition.y + (m_data.power * Mathf::RandomRange(-1.0f, 1.0f) * sinf(m_data.timer));
		break;
	}

	return m_data.isEnd;
}
