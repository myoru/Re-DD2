#pragma once

#include <DirectXMath.h>
#include <utility>

//�q�b�g����
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0,0,0 };//���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3 normal = { 0,0,0 };//�Փ˂����|���S���̖@���x�N�g��
	float distance = 0.0f;//���C�̎n�_�����_�܂ł̋���
	int materialIndex = -1;//�Փ˂����|���S���̃}�e���A���ԍ�
	DirectX::XMFLOAT3 rotation = { 0,0,0 };//��]��
};




//�R���W����
class Collision
{
public:
	//���Ƌ��̌�������
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	//���Ɖ~���̌�������
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);

	////�X�N���[����Ԃł̔���
	//static bool IntersectSquareVsSquareScreen(
	//	const DirectX::XMFLOAT2& squareLeftTopA,
	//	const DirectX::XMFLOAT2& squareRightBottomA,
	//	const DirectX::XMFLOAT2& squareLeftTopB,
	//	const DirectX::XMFLOAT2& squareRightBottomB
	//);

	//static bool IntersectSquareVsSquareWorld(
	//	const DirectX::XMFLOAT2& squareLeftTopA,
	//	const DirectX::XMFLOAT2& squareRightBottomA,
	//	const DirectX::XMFLOAT2& squareLeftTopB,
	//	const DirectX::XMFLOAT2& squareRightBottomB
	//);

	//��`�Ƌ�`�̓��蔻��(��������)
	/*static bool IntersectSquareVsSquareScreen(
		const DirectX::XMFLOAT2& squareLeftTopA,
		const DirectX::XMFLOAT2& squareRightBottomA,
		const DirectX::XMFLOAT2& squareLeftTopB,
		const DirectX::XMFLOAT2& squareRightBottomB
	);*/
};
