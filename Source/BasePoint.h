#pragma once
#include <string>
#include <DirectXMath.h>

enum class BasePoint
{
	Center,//����
	LeftTop,//����
	RightTop,//�E��
	LeftBottom,//����
	RightBottom,//�E��
	Top,//������
	Bottom,//������
	Left,//��
	Right,//�E
	Max = Right,
};

static std::string GetBasePointStr(int a_basePointIndex)
{
	switch (static_cast<BasePoint>(a_basePointIndex))
	{
	case BasePoint::Center://�^��
		return "Center";
	case BasePoint::LeftTop://����
		return "LeftTop";
	case BasePoint::RightTop://�E��
		return "RightTop";
	case BasePoint::LeftBottom://����
		return "LeftBottom";
	case BasePoint::RightBottom://�E��
		return "RightBottom";
	case BasePoint::Top://������
		return "Top";
	case BasePoint::Bottom://������
		return "Bottom";
	case BasePoint::Left://��
		return "Left";
	case BasePoint::Right://�E
		return "Right";
	}
	return "None";
}

//(�X�N���[���X�y�[�X)��`�̒����̍��W���擾����
static DirectX::XMFLOAT2 CalcsSquareCenterPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardCenterPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://�^��
		_boardCenterPosition = a_boarBasePosition; break;
	case BasePoint::LeftTop://����
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::RightTop://�E��
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftBottom://����
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::RightBottom://�E��
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Top://������
		_boardCenterPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Bottom://������
		_boardCenterPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Left://��
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Right://�E
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	}

	return _boardCenterPosition;
}

//(�X�N���[���X�y�[�X)��`�̍����̍��W���擾����
static DirectX::XMFLOAT2 CalcSquareLeftBottomPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardLeftBottomPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://�^��
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://����
		_boardLeftBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::RightTop://�E��
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::LeftBottom://����
		_boardLeftBottomPosition = a_boarBasePosition; break;
	case BasePoint::RightBottom://�E��
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::Top://������
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::Bottom://������
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Left://��
		_boardLeftBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://�E
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardLeftBottomPosition;
}

//(�X�N���[���X�y�[�X)��`�̍���̍��W���擾����
static DirectX::XMFLOAT2 CalcSquareLeftTopPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardLeftTopPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://�^��
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://����
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y}; break;
	case BasePoint::RightTop://�E��
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y}; break;
	case BasePoint::LeftBottom://����
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y}; break;
	case BasePoint::RightBottom://�E��
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Top://������
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Bottom://������
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Left://��
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://�E
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardLeftTopPosition;
}

//(�X�N���[���X�y�[�X)��`�̉E��̍��W���擾����
static DirectX::XMFLOAT2 CalcSquareRightTopPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardRightTopPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://�^��
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://����
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::RightTop://�E��
		_boardRightTopPosition = a_boarBasePosition; break;
	case BasePoint::LeftBottom://����
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::RightBottom://�E��
		_boardRightTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Top://������
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Bottom://������
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Left://��
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://�E
		_boardRightTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardRightTopPosition;
}

//(�X�N���[���X�y�[�X)��`�̉E���̍��W���擾����
static DirectX::XMFLOAT2 CalcSquareRightBottomPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardRightBottomPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://�^��
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://����
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::RightTop://�E��
		_boardRightBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::LeftBottom://����
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::RightBottom://�E��
		_boardRightBottomPosition = a_boarBasePosition; break;
	case BasePoint::Top://������
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::Bottom://������
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Left://��
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://�E
		_boardRightBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardRightBottomPosition;
}