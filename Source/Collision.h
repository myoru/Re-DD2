#pragma once

#include <DirectXMath.h>
#include <utility>

//ヒット結果
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交点
	DirectX::XMFLOAT3 normal = { 0,0,0 };//衝突したポリゴンの法線ベクトル
	float distance = 0.0f;//レイの始点から交点までの距離
	int materialIndex = -1;//衝突したポリゴンのマテリアル番号
	DirectX::XMFLOAT3 rotation = { 0,0,0 };//回転量
};




//コリジョン
class Collision
{
public:
	//球と球の交差判定
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	//球と円柱の交差判定
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);

	////スクリーン空間での判定
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

	//矩形と矩形の当り判定(押し合い)
	/*static bool IntersectSquareVsSquareScreen(
		const DirectX::XMFLOAT2& squareLeftTopA,
		const DirectX::XMFLOAT2& squareRightBottomA,
		const DirectX::XMFLOAT2& squareLeftTopB,
		const DirectX::XMFLOAT2& squareRightBottomB
	);*/
};
