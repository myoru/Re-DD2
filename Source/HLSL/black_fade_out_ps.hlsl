#include "fullscreen_quad.hlsli"

cbuffer BLACK_FADE_CONSTANT__BUFFER : register(b3)
{
	float time;
	float fade_value;
	int in_or_out;
	int start;
	int black_or_white;
}

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEAR_BORDER_BLACK 3
#define LINEAR_BORDER_WHITE 4
SamplerState sampler_states[7] : register(s0);
Texture2D texture_maps[4] : register(t0);
float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color;
	switch (black_or_white)
	{
		case 0:
			color = texture_maps[0].Sample(sampler_states[LINEAR_BORDER_BLACK], pin.texcoord);
			break;
		case 1:
			color = texture_maps[0].Sample(sampler_states[LINEAR_BORDER_WHITE], pin.texcoord);
			break;
		default:
			color = texture_maps[0].Sample(sampler_states[LINEAR], pin.texcoord);
			break;
		
	}
	float alpha = color.w;
	
	alpha *= fade_value;
	
	return float4(color.rgb, alpha);
}