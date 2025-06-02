#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE, COMPARISON_LINEAR_BORDER_WHITE ,RANP/*, CASCADED_SHADOW_MAPS*/ /*linear_border_black, linear_border_white, linear_mirror, comparison_linear_border_white*/ };
enum class DEPTH_STENCIL_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF, SILHOUETTE, OUTLINE };
enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
enum class RASTERIZER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };

class RenderingState
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[7];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[7];
	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[5];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[5];

public:
	RenderingState(ID3D11Device* device);
	virtual ~RenderingState() = default;

	void SetDepthStencilState(ID3D11DeviceContext* immediate_context, DEPTH_STENCIL_STATE depth_stencil_state, UINT stencil_ref = 0)
	{
		immediate_context->OMSetDepthStencilState(depth_stencil_states[static_cast<size_t>(depth_stencil_state)].Get(), stencil_ref);
	}
	void set_sampler_states(ID3D11DeviceContext* immediate_context)
	{
		std::vector<ID3D11SamplerState*> samplers;
		for (const Microsoft::WRL::ComPtr<ID3D11SamplerState>& sampler : sampler_states)
		{
			samplers.emplace_back(sampler.Get());
		}
		immediate_context->PSSetSamplers(0, static_cast<UINT>(samplers.size()), samplers.data());
		immediate_context->GSSetSamplers(0, static_cast<UINT>(samplers.size()), samplers.data());
	}
	void SetBlendState(ID3D11DeviceContext* immediate_context, BLEND_STATE blend_state)
	{
		immediate_context->OMSetBlendState(blend_states[static_cast<size_t>(blend_state)].Get(), NULL, 0xFFFFFFFF);
	}
	void SetRasterizerState(ID3D11DeviceContext* immediate_context, RASTERIZER_STATE rasterizer_state)
	{
		immediate_context->RSSetState(rasterizer_states[static_cast<size_t>(rasterizer_state)].Get());
	}

	ID3D11DepthStencilState* depth_stencil_state(DEPTH_STENCIL_STATE depth_stencil_state)
	{
		return depth_stencil_states[static_cast<size_t>(depth_stencil_state)].Get();
	}
	ID3D11BlendState* blend_state(BLEND_STATE blend_state)
	{
		return blend_states[static_cast<size_t>(blend_state)].Get();
	}
	ID3D11RasterizerState* rasterizer_state(RASTERIZER_STATE rasterizer_state)
	{
		return rasterizer_states[static_cast<size_t>(rasterizer_state)].Get();
	}

private:
};