#pragma once
#include <d3d11.h>
#include <vector>

HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename,
	ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc = nullptr);

HRESULT load_texture_from_memory(ID3D11Device* device, const void* data, size_t size,
	ID3D11ShaderResourceView** shader_resource_view);

void Release_All_Texture();

HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension);