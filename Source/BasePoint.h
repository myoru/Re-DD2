#pragma once
#include <string>
#include <DirectXMath.h>

enum class BasePoint
{
	Center,//中央
	LeftTop,//左上
	RightTop,//右上
	LeftBottom,//左下
	RightBottom,//右下
	Top,//中央上
	Bottom,//中央下
	Left,//左
	Right,//右
	Max = Right,
};

static std::string GetBasePointStr(int a_basePointIndex)
{
	switch (static_cast<BasePoint>(a_basePointIndex))
	{
	case BasePoint::Center://真ん中
		return "Center";
	case BasePoint::LeftTop://左上
		return "LeftTop";
	case BasePoint::RightTop://右上
		return "RightTop";
	case BasePoint::LeftBottom://左下
		return "LeftBottom";
	case BasePoint::RightBottom://右下
		return "RightBottom";
	case BasePoint::Top://中央上
		return "Top";
	case BasePoint::Bottom://中央下
		return "Bottom";
	case BasePoint::Left://左
		return "Left";
	case BasePoint::Right://右
		return "Right";
	}
	return "None";
}

//(スクリーンスペース)矩形の中央の座標を取得する
static DirectX::XMFLOAT2 CalcsSquareCenterPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardCenterPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://真ん中
		_boardCenterPosition = a_boarBasePosition; break;
	case BasePoint::LeftTop://左上
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::RightTop://右上
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftBottom://左下
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::RightBottom://右下
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Top://中央上
		_boardCenterPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Bottom://中央下
		_boardCenterPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Left://左
		_boardCenterPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Right://右
		_boardCenterPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	}

	return _boardCenterPosition;
}

//(スクリーンスペース)矩形の左下の座標を取得する
static DirectX::XMFLOAT2 CalcSquareLeftBottomPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardLeftBottomPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://真ん中
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://左上
		_boardLeftBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::RightTop://右上
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::LeftBottom://左下
		_boardLeftBottomPosition = a_boarBasePosition; break;
	case BasePoint::RightBottom://右下
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::Top://中央上
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::Bottom://中央下
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Left://左
		_boardLeftBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://右
		_boardLeftBottomPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardLeftBottomPosition;
}

//(スクリーンスペース)矩形の左上の座標を取得する
static DirectX::XMFLOAT2 CalcSquareLeftTopPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardLeftTopPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://真ん中
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://左上
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y}; break;
	case BasePoint::RightTop://右上
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y}; break;
	case BasePoint::LeftBottom://左下
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y}; break;
	case BasePoint::RightBottom://右下
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Top://中央上
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Bottom://中央下
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Left://左
		_boardLeftTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://右
		_boardLeftTopPosition = { a_boarBasePosition.x - a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardLeftTopPosition;
}

//(スクリーンスペース)矩形の右上の座標を取得する
static DirectX::XMFLOAT2 CalcSquareRightTopPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardRightTopPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://真ん中
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://左上
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::RightTop://右上
		_boardRightTopPosition = a_boarBasePosition; break;
	case BasePoint::LeftBottom://左下
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::RightBottom://右下
		_boardRightTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Top://中央上
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Bottom://中央下
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y - a_boardDrawSize.y }; break;
	case BasePoint::Left://左
		_boardRightTopPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://右
		_boardRightTopPosition = { a_boarBasePosition.x,a_boarBasePosition.y - a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardRightTopPosition;
}

//(スクリーンスペース)矩形の右下の座標を取得する
static DirectX::XMFLOAT2 CalcSquareRightBottomPosition(BasePoint a_basePoint, DirectX::XMFLOAT2 a_boarBasePosition, DirectX::XMFLOAT2 a_boardDrawSize)
{
	DirectX::XMFLOAT2 _boardRightBottomPosition{};
	switch (a_basePoint)
	{
	case BasePoint::Center://真ん中
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::LeftTop://左上
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::RightTop://右上
		_boardRightBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::LeftBottom://左下
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y }; break;
	case BasePoint::RightBottom://右下
		_boardRightBottomPosition = a_boarBasePosition; break;
	case BasePoint::Top://中央上
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y + a_boardDrawSize.y }; break;
	case BasePoint::Bottom://中央下
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x / 2.0f,a_boarBasePosition.y }; break;
	case BasePoint::Left://左
		_boardRightBottomPosition = { a_boarBasePosition.x + a_boardDrawSize.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	case BasePoint::Right://右
		_boardRightBottomPosition = { a_boarBasePosition.x,a_boarBasePosition.y + a_boardDrawSize.y / 2.0f }; break;
	}

	return _boardRightBottomPosition;
}