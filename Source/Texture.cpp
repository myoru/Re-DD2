#include "Texture.h"
#include <WICTextureLoader.h>


using namespace DirectX;

#include "Misc.h"
#include <wrl.h>

using namespace Microsoft::WRL;

#include <string>
#include <map>
using namespace std;

#include <sstream>
#include <iomanip>

#include <filesystem>
#include <DDSTextureLoader.h>

static map<wstring, ComPtr<ID3D11ShaderResourceView>> resources;

HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename,
	ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
	HRESULT hr{ S_OK };
	ComPtr<ID3D11Resource> resource;

	auto it = resources.find(filename);
	if (it != resources.end())
	{
		*shader_resource_view = it->second.Get();
		(*shader_resource_view)->AddRef();
		(*shader_resource_view)->GetResource(resource.GetAddressOf());
	}
	else
	{
		std::filesystem::path dds_filename(filename);
		dds_filename.replace_extension("dds");
		if (std::filesystem::exists(dds_filename.c_str()))
		{
			hr = CreateDDSTextureFromFile(device, dds_filename.c_str(), resource.GetAddressOf(), shader_resource_view);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		else
		{
			hr = CreateWICTextureFromFile(device, filename, resource.GetAddressOf(), shader_resource_view);
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		resources.insert(make_pair(filename, *shader_resource_view));
	}

	if (texture2d_desc)
	{
		ComPtr<ID3D11Texture2D> texture2d;
		hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		texture2d->GetDesc(texture2d_desc);
	}

	return hr;
}

HRESULT load_texture_from_memory(ID3D11Device* device, const void* data, size_t size, ID3D11ShaderResourceView** shader_resource_view)
{
	HRESULT hr{ S_OK };
	ComPtr<ID3D11Resource> resource;

	hr = CreateDDSTextureFromMemory(device, reinterpret_cast<const uint8_t*>(data),
		size, resource.GetAddressOf(), shader_resource_view);
	if (hr != S_OK)
	{
		hr = CreateWICTextureFromMemory(device, reinterpret_cast<const uint8_t*>(data),
			size, resource.GetAddressOf(), shader_resource_view);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	return hr;
}

void Release_All_Texture()
{
	resources.clear();
}

HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value, UINT dimension)
{
	HRESULT hr{ S_OK };

	wstringstream keyname;
	keyname << setw(8) << setfill(L'0') << hex << uppercase << value << L"." << dec << dimension;
	map<wstring, ComPtr<ID3D11ShaderResourceView>>::iterator it = resources.find(keyname.str().c_str());
	if (it != resources.end())
	{
		*shader_resource_view = it->second.Get();
		(*shader_resource_view)->AddRef();
	}
	else
	{
		D3D11_TEXTURE2D_DESC texture2d_desc{};
		texture2d_desc.Width = dimension;
		texture2d_desc.Height = dimension;
		texture2d_desc.MipLevels = 1;
		texture2d_desc.ArraySize = 1;
		texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
		texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture2d_desc.CPUAccessFlags = 0;
		texture2d_desc.MiscFlags = 0;

		size_t texels = static_cast<size_t>(dimension) * dimension;
		unique_ptr<DWORD[]> sysmem{ make_unique< DWORD[]>(texels) };
		for (size_t i = 0; i < texels; ++i)
		{
			sysmem[i] = value;
		}

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = sysmem.get();
		subresource_data.SysMemPitch = sizeof(DWORD) * dimension;
		subresource_data.SysMemSlicePitch = 0;

		ComPtr<ID3D11Texture2D> texture2d;
		hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
		shader_resource_view_desc.Format = texture2d_desc.Format;
		shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shader_resource_view_desc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shader_resource_view);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		resources.insert(std::make_pair(keyname.str().c_str(), *shader_resource_view));
	}
	return hr;
}
