#include "Collision.h"

bool Collision::IntersectCylinderVsCylinder(const DirectX::XMFLOAT3& positionA, float radiusA, float heightA, const DirectX::XMFLOAT3& positionB, float radiusB, float heightB, DirectX::XMFLOAT3& outPositionB)
{
	//Aの足元がBの頭より上なら当っていない
	if (positionA.y > positionB.y + heightB)
	{
		return false;
	}
	//Aの頭がBの足元より下なら当っていない
	if (positionA.y + heightA < positionB.y)
	{
		return false;
	}
	//XZ平面での範囲チェック
	DirectX::XMFLOAT3 posA = positionA;
	DirectX::XMFLOAT3 posB = positionB;
	posA.y = posB.y = 0.0f;
	DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&posA);
	DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&posB);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(PositionB, PositionA);
	DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);//Sqを付けるとsqrtfする前の値が返ってくる
	float lengthSq;
	DirectX::XMStoreFloat(&lengthSq, LengthSq);

	float range = radiusA + radiusB;
	if (range * range <= lengthSq)
	{
		return false;
	}
	//AがBを押し出す
	Vec = DirectX::XMVector3Normalize(Vec);
	PositionB = DirectX::XMVectorAdd(PositionA, DirectX::XMVectorScale(Vec, range));

	DirectX::XMStoreFloat3(&outPositionB, PositionB);

	outPositionB.y = positionB.y;

	return true;
}

bool Collision::IntersectSphereVsCylinder(const DirectX::XMFLOAT3& spherePosition, float sphereRadius, const DirectX::XMFLOAT3& cylinderPosition, float cylinderRadius, float cylinderHeight, DirectX::XMFLOAT3& outCylinderPosition)
{
	DirectX::XMFLOAT3 heightPoint = { cylinderPosition.x ,cylinderPosition.y + cylinderHeight,cylinderPosition.z };
	DirectX::XMVECTOR posHeight = DirectX::XMLoadFloat3(&heightPoint);
	DirectX::XMVECTOR posCylinder = DirectX::XMLoadFloat3(&cylinderPosition);
	DirectX::XMVECTOR posSphere = DirectX::XMLoadFloat3(&spherePosition);
	DirectX::XMVECTOR VecHeight = DirectX::XMVectorSubtract(posHeight, posCylinder);
	float lenHeight = DirectX::XMVectorGetX(DirectX::XMVector3Length(VecHeight));
	VecHeight = DirectX::XMVector3Normalize(VecHeight);
	DirectX::XMVECTOR Vecsphere = DirectX::XMVectorSubtract(posSphere, posCylinder);
	DirectX::XMVECTOR dotSphereHeight = DirectX::XMVector3Dot(VecHeight, Vecsphere);
	float lenDot = DirectX::XMVectorGetX(dotSphereHeight);

	//上下判定
	if (heightPoint.y + sphereRadius < spherePosition.y)
	{
		return false;
	}
	if (cylinderPosition.y - sphereRadius > spherePosition.y)
	{
		return false;
	}

	DirectX::XMVECTOR d = DirectX::XMVectorAdd(posCylinder, DirectX::XMVectorScale(VecHeight, lenDot));
	DirectX::XMVECTOR VecD = DirectX::XMVectorSubtract(d, posSphere);
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(VecD));
	VecD = DirectX::XMVector3Normalize(VecD);

	if (sphereRadius + cylinderRadius < distance)
	{
		return false;
	}

	DirectX::XMVECTOR outPosition = DirectX::XMVectorAdd(posSphere, DirectX::XMVectorScale(VecD, sphereRadius + cylinderRadius));

	DirectX::XMStoreFloat3(&outCylinderPosition, outPosition);

	return true;
}

////スクリーン空間での判定
//bool Collision::IntersectSquareVsSquareScreen(const DirectX::XMFLOAT2& squareLeftTopA, const DirectX::XMFLOAT2& squareRightBottomA, const DirectX::XMFLOAT2& squareLeftTopB, const DirectX::XMFLOAT2& squareRightBottomB)
//{
//	if (squareLeftTopA.x > squareRightBottomB.x && squareRightBottomA.x < squareLeftTopB.x &&
//		squareLeftTopA.y > squareRightBottomB.y && squareRightBottomA.y < squareLeftTopB.y)
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//}
//bool Collision::IntersectSquareVsSquareWorld(const DirectX::XMFLOAT2& squareLeftTopA, const DirectX::XMFLOAT2& squareRightBottomA, const DirectX::XMFLOAT2& squareLeftTopB, const DirectX::XMFLOAT2& squareRightBottomB)
//{
//	if (squareLeftTopA.x > squareRightBottomB.x || squareRightBottomA.x < squareLeftTopB.x ||
//		squareLeftTopA.y < squareRightBottomB.y || squareRightBottomA.y > squareLeftTopB.y)
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//}


