#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <assert.h>

#include "Misc.h"

#define USAGE_DYNAMIC

enum class CB_USAGE : byte
{
	V = 0x01,
	H = 0x02,
	D = 0x04,
	G = 0x08,
	P = 0x10,
	C = 0x20,
	VP = V | P,
	VGP = V | G | P,
	VHDP = V | H | D | P,
	VHDGP = V | H | D | G | P,
	VHDGPC = V | H | D | G | P | C,
};

template <class T>
struct ConstantBuffer
{
	T data{};
	ConstantBuffer(ID3D11Device* device)
	{
#if 0
		assert(sizeof(T) % 16 == 0 && L"constant buffer's need to be 16 byte aligned");
#endif

		HRESULT hr = S_OK;
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = (sizeof(T) + 0x0f) & ~0x0f;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
#ifdef USAGE_DYNAMIC
		//https://docs.microsoft.com/en-us/windows/desktop/direct3d11/how-to--use-dynamic-resources
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = device->CreateBuffer(&buffer_desc, 0, buffer_object.GetAddressOf());
		assert(SUCCEEDED(hr) && hr_trace(hr));
#else
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.CPUAccessFlags = 0;
	/*	D3D11_SUBRESOURCE_DATA subresource_data;
		subresource_data.pSysMem = &data;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;*/
		hr = device->CreateBuffer(&buffer_desc, nullptr, buffer_object.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif
	}
	virtual ~ConstantBuffer() = default;
	ConstantBuffer(ConstantBuffer&) = delete;
	ConstantBuffer& operator =(ConstantBuffer&) = delete;

	void activate(ID3D11DeviceContext* immediate_context, int slot, CB_USAGE usage)
	{
		activate(immediate_context, slot, usage, &data);
	}
	void activate(ID3D11DeviceContext* immediate_context, int slot, CB_USAGE usage, const T* data)
	{
		HRESULT hr = S_OK;
#ifdef USAGE_DYNAMIC
		D3D11_MAP map = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mapped_buffer;

		hr = immediate_context->Map(buffer_object.Get(), 0, map, 0, &mapped_buffer);
		assert(SUCCEEDED(hr) && hr_trace(hr));
		memcpy_s(mapped_buffer.pData, sizeof(T), data, sizeof(T));
		immediate_context->Unmap(buffer_object.Get(), 0);
#else
		immediate_context->UpdateSubresource(buffer_object.Get(), 0, 0, &data, 0, 0);
#endif
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::V))
		{
			immediate_context->VSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::H))
		{
			immediate_context->HSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::D))
		{
			immediate_context->DSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::G))
		{
			immediate_context->GSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::P))
		{
			immediate_context->PSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
		if (static_cast<byte>(usage) & static_cast<byte>(CB_USAGE::C))
		{
			immediate_context->CSSetConstantBuffers(slot, 1, buffer_object.GetAddressOf());
		}
	}
	void deactivate(ID3D11DeviceContext* immediate_context)
	{
		// NOP
	}
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer_object;
};
