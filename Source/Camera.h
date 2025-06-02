#pragma once

#include <algorithm>
#include <DirectXMath.h>

//カメラ
class Camera
{
private:
	Camera() {}
	~Camera() {}

public:
	//唯一のインスタンス取得
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	void Update(float elapsedTime);

	//指定方向を向く
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
		const DirectX::XMFLOAT3& up);

	//パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	void SetOrthographic(float width, float height, float nearZ, float farZ);

	void SetHybridProjection(float fovY, float aspect, float width, float height, float nearZ, float farZ, float rate);

	//ビュー行列取得
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	//プロジェクション行列取得
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }
	const DirectX::XMFLOAT4X4& GetPerspectiveProjection() const { return perspective_projection; }
	const DirectX::XMFLOAT4X4& GetParallelProjection() const { return parallel_projection; }

	////視点取得
	//const DirectX::XMFLOAT4& GetEye() const { return eye; }

	////注視点
	//void SetFocus(DirectX::XMFLOAT4 target) {focus = target;}
	//const DirectX::XMFLOAT4& GetFocus() const { return focus; }

	////上方向取得
	//const DirectX::XMFLOAT3& GetUp() const { return up; }

	////前方向取得
	//const DirectX::XMFLOAT3& GetFront() const { return front; }

	////右方向取得
	//const DirectX::XMFLOAT3& GetRight() const { return right; }

	void SetRollSpeed(DirectX::XMFLOAT3 speed) { rollSpeed = speed; }
	void SetAddRange(float add) { add_range = add; }
	/*void SetRange(float newRange) { range = newRange; }*/

private:
	DirectX::XMFLOAT4X4 view{};
	DirectX::XMFLOAT4X4 projection{};
	DirectX::XMFLOAT4X4 perspective_projection{};
	DirectX::XMFLOAT4X4 parallel_projection{};

public:
	DirectX::XMFLOAT3 eye = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 focus = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	DirectX::XMFLOAT3 front = { 0.0f,0.0f,1.0f };
	DirectX::XMFLOAT3 right = { 1.0f,0.0f,0.0f };

	float range = 10.0f;
public:
	DirectX::XMFLOAT3 angle = { 0.22f,0,0 };
	DirectX::XMFLOAT3 rollSpeed = {};
	float add_range = 0;
	float maxAngleX = DirectX::XMConvertToRadians(75);
	float minAngleX = DirectX::XMConvertToRadians(-75);
};
