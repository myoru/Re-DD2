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

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT4 front;
	DirectX::XMStoreFloat4(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
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

//�w�����������
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

	//�J�����̕��������o��
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	//���_�A�����_��ۑ�
	this->eye = eye;
	this->focus = focus;
}

//�p�[�X�y�N�e�B�u�ݒ�
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	//DirectX::XMStoreFloat4x4(&projection, Projection);
	DirectX::XMStoreFloat4x4(&perspective_projection, Projection);
}

void Camera::SetOrthographic(float width, float height, float nearZ, float farZ)
{
	//��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s����쐬
	DirectX::XMMATRIX Projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
	DirectX::XMStoreFloat4x4(&parallel_projection, Projection);
}

void Camera::SetHybridProjection(float fovY, float aspect, float width, float height, float nearZ, float farZ, float rate)
{
	//�������e�̍s��쐬
	DirectX::XMMATRIX _perspective_projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);

	//���s���e�̍s��쐬
	DirectX::XMMATRIX _parallel_projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);

	//�n�C�u���b�h���e�̍s��쐬�֐�
	auto _LerpMatrix = [](const DirectX::XMMATRIX& matrix1, const DirectX::XMMATRIX& matrix2, float t)->DirectX::XMMATRIX
		{
			// �s�񂩂�X�P�[���A��]�A�ʒu�𒊏o
			DirectX::XMVECTOR scale1, rotation1, translation1;
			DirectX::XMMatrixDecompose(&scale1, &rotation1, &translation1, matrix1);

			DirectX::XMVECTOR scale2, rotation2, translation2;
			DirectX::XMMatrixDecompose(&scale2, &rotation2, &translation2, matrix2);

			// �X�P�[���A��]�A�ʒu����`���
			DirectX::XMVECTOR lerpedScale = DirectX::XMVectorLerp(scale1, scale2, t);
			DirectX::XMVECTOR lerpedRotation = DirectX::XMQuaternionSlerp(rotation1, rotation2, t); // ���ʐ��`���
			DirectX::XMVECTOR lerpedTranslation = DirectX::XMVectorLerp(translation1, translation2, t);

			// ��Ԃ��ꂽ�X�P�[���A��]�A�ʒu����s����č\��
			return DirectX::XMMatrixScalingFromVector(lerpedScale) *
				DirectX::XMMatrixRotationQuaternion(lerpedRotation) *
				DirectX::XMMatrixTranslationFromVector(lerpedTranslation);
		};

	//�n�C�u���b�h���e�̍s��쐬
	DirectX::XMMATRIX _hybrid_projection = _LerpMatrix(_perspective_projection, _parallel_projection, rate);

	DirectX::XMStoreFloat4x4(&projection, _hybrid_projection);
}


