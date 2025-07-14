#include "sprite.hlsli"

cbuffer CameraConstants : register(b3)
{
	row_major float4x4 world;
	row_major float4x4 viewProjection;
}

VS_OUT main(float4 position : POSITION, float4 color : COLOR,float2 texcoord : TEXCOORD)
{
	VS_OUT vout;
	
	// ローカル → ワールド → ビュー射影（最終スクリーン座標）
	float4 worldPos = mul(position, world);
	vout.position = mul(worldPos, viewProjection);

	vout.color = color;
	vout.texcoord = texcoord;

	return vout;
}