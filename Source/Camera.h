#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

class Camera
{
public:
	Camera() {}
	~Camera() {}

	void Update(DirectX::XMFLOAT2 a_targetPos, DirectX::XMFLOAT2 a_screenSize)
	{
		eye = a_targetPos;
		viewMatrix = GetViewProjMatrix(a_screenSize.x, a_screenSize.y);
	}

	DirectX::XMMATRIX GetViewProjMatrix(float screenWidth, float screenHeight) const
	{
		DirectX::XMMATRIX view = DirectX::XMMatrixTranslation(-eye.x, -eye.y, 0.0f);
		DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(screenWidth / zoom, screenHeight / zoom, 0.0f, 1.0f);
		return view * proj;
	}

	DirectX::XMMATRIX viewMatrix = {};
	DirectX::XMFLOAT2 eye = { 0.0f, 0.0f };
	float zoom = 1.0f;
};