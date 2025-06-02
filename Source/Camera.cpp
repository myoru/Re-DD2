#include "Camera.h"

void Camera::Update(float elapsedTime)
{
	DirectX::XMFLOAT3 speed = { rollSpeed.x,
								rollSpeed.y,
								rollSpeed.z };
	angle.x += speed.x;
	angle.y += speed.y;

	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}

	range += add_range * elapsedTime;

	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT4 front;
	DirectX::XMStoreFloat4(&front, Front);

	//注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = focus.x - front.x * range;
	eye.y = (focus.y - front.y * range)/* * 0.5f*/;
	eye.z = focus.z - front.z * range;

	if (eye.x == focus.x && eye.y == focus.y && eye.z == focus.z)
	{
		eye.z -= front.z * 0.001f;
	}

	add_range = 0;

	Camera::Instance().SetLookAt(eye, focus, DirectX::XMFLOAT3(0, 1, 0));
}

//指定方向を向く
void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
	const DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	//カメラの方向を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	//視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

//パースペクティブ設定
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//画角、画面比率、クリップ距離からプロジェクション行列を作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	//DirectX::XMStoreFloat4x4(&projection, Projection);
	DirectX::XMStoreFloat4x4(&perspective_projection, Projection);
}

void Camera::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	//画角、画面比率、クリップ距離からプロジェクション行列を作成
	DirectX::XMMATRIX Projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
	DirectX::XMStoreFloat4x4(&parallel_projection, Projection);
}

void Camera::SetHybridProjection(float fovY, float aspect, float width, float height, float nearZ, float farZ, float rate)
{
	//透視投影の行列作成
	DirectX::XMMATRIX _perspective_projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);

	//平行投影の行列作成
	DirectX::XMMATRIX _parallel_projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);

	//ハイブリッド投影の行列作成関数
	auto _LerpMatrix = [](const DirectX::XMMATRIX& matrix1, const DirectX::XMMATRIX& matrix2, float t)->DirectX::XMMATRIX
		{
			// 行列からスケール、回転、位置を抽出
			DirectX::XMVECTOR scale1, rotation1, translation1;
			DirectX::XMMatrixDecompose(&scale1, &rotation1, &translation1, matrix1);

			DirectX::XMVECTOR scale2, rotation2, translation2;
			DirectX::XMMatrixDecompose(&scale2, &rotation2, &translation2, matrix2);

			// スケール、回転、位置を線形補間
			DirectX::XMVECTOR lerpedScale = DirectX::XMVectorLerp(scale1, scale2, t);
			DirectX::XMVECTOR lerpedRotation = DirectX::XMQuaternionSlerp(rotation1, rotation2, t); // 球面線形補間
			DirectX::XMVECTOR lerpedTranslation = DirectX::XMVectorLerp(translation1, translation2, t);

			// 補間されたスケール、回転、位置から行列を再構成
			return DirectX::XMMatrixScalingFromVector(lerpedScale) *
				DirectX::XMMatrixRotationQuaternion(lerpedRotation) *
				DirectX::XMMatrixTranslationFromVector(lerpedTranslation);
		};

	//ハイブリッド投影の行列作成
	DirectX::XMMATRIX _hybrid_projection = _LerpMatrix(_perspective_projection, _parallel_projection, rate);

	DirectX::XMStoreFloat4x4(&projection, _hybrid_projection);
}


