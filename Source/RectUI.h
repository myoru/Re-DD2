#pragma once
#include <DirectXMath.h>
#include <memory>
#include <algorithm>
#include "Sprite.h"
#include "Mouse.h"


class RectUI
{
public:
	RectUI(const wchar_t* a_fileName, BasePoint a_basePoint = BasePoint::Center);
	~RectUI() {}

	void Update(float a_elapsedTime)
	{
		leftTopPos = CalcSquareLeftTopPosition(sprite->GetBasePoint(), position, size);
		rightBottomPos = CalcSquareRightBottomPosition(sprite->GetBasePoint(), position, size);
		doubleClickTimer -= a_elapsedTime;
		if (doubleClickTimer <= 0.0f)
		{
			clickCount = 0;
			doubleClickTimer = 0.0f;
		}
	}

	void Render(DirectX::XMFLOAT4 a_color = { 1.0f,1.0f,1.0f,1.0f })
	{
		sprite->Render(sprite->GetBasePoint(), position, size, 0.0f, a_color);
	}
	void Render(DirectX::XMFLOAT2 a_scale,DirectX::XMFLOAT4 a_color = { 1.0f,1.0f,1.0f,1.0f })
	{
		sprite->Render(sprite->GetBasePoint(), position, { size.x * a_scale.x,size.y * a_scale.y }, 0.0f, a_color);
	}
	bool MouseHitCheck(const Mouse& a_mouse)
	{
		if (a_mouse.GetPositionX() < leftTopPos.x || rightBottomPos.x < a_mouse.GetPositionX() ||
			a_mouse.GetPositionY() < leftTopPos.y || rightBottomPos.y < a_mouse.GetPositionY())
		{
			mouseTouchFlag = false;
			return false;
		}
		else
		{
			mouseTouchFlag = true;
		}
		if (a_mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			clickCount++;
			if (clickCount >= 2)
			{
				clickCount = 0;
				doubleClickTimer = 0.0f;
				return true;
			}
			doubleClickTimer = 0.5f;
		}
		return false;
	}

	std::unique_ptr<Sprite> sprite;
	DirectX::XMFLOAT2 position{};
	DirectX::XMFLOAT2 leftTopPos{};
	DirectX::XMFLOAT2 rightBottomPos{};
	DirectX::XMFLOAT2 size{};
	bool mouseTouchFlag = false;
	float doubleClickTimer = 0.0f;
	int clickCount = 0;
};
