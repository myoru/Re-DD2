#include "SameSprite.h"
#include "Graphics.h"
#include "Texture.h"
#include "Shader.h"
#include "Misc.h"
#include <sstream>
#include <WICTextureLoader.h>

SameSprite::SameSprite(const wchar_t* filename, BasePoint c_basePoint, size_t max_sprites)
	:max_vertices(max_sprites * 6), base_point(c_basePoint)
{
	ID3D11Device* _device = Graphics::Instance().GetDevice();

	HRESULT hr{ S_OK };

	std::unique_ptr<vertex[]> vertices{ std::make_unique<vertex[]>(max_vertices) };

	//頂点バッファオブジェクト生成
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = static_cast <UINT>(sizeof(vertex) * max_vertices);
	//バッファーの読み取りと書き込み方法の設定
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = vertices.get();
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = _device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//入力レイアウトオブジェクト生成
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateVsFromCso(_device, ".\\Data\\Shader\\sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	CreatePsFromCso(_device, ".\\Data\\Shader\\sprite_ps.cso", pixel_shader.GetAddressOf());

	if (filename)
	{
		LoadTextureFromFile(_device, filename, shader_resource_view.GetAddressOf(), &texture2d_desc);
	}
	else
	{
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		texture2d_desc.Width = width;
		texture2d_desc.Height = height;
		texture2d_desc.MipLevels = 1;
		texture2d_desc.ArraySize = 1;
		texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
		texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture2d_desc.CPUAccessFlags = 0;
		texture2d_desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
		HRESULT hr = _device->CreateTexture2D(&texture2d_desc, &data, texture.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = _device->CreateShaderResourceView(texture.Get(), nullptr, shader_resource_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	textureWidth = static_cast<float>(texture2d_desc.Width);
	textureHeight = static_cast<float>(texture2d_desc.Height);
}

SameSprite::~SameSprite()
{
}

void SameSprite::Begin()
{
	ID3D11DeviceContext* _immediate_context = Graphics::Instance().GetDeviceContext();

	vertices.clear();

	//シェーダーのバインド
	_immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	_immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	//シェーダーリソースのバインド
	_immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
}

void SameSprite::End()
{
	ID3D11DeviceContext* _immediate_context = Graphics::Instance().GetDeviceContext();

	//頂点バッファオブジェクトを更新
	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = _immediate_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	size_t vertex_count = vertices.size();
	_ASSERT_EXPR(max_vertices >= vertex_count, "Buffer overflow");
	vertex* data{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (data != nullptr)
	{
		const vertex* p = vertices.data();
		memcpy_s(data, max_vertices * sizeof(vertex), p, vertex_count * sizeof(vertex));
	}
	_immediate_context->Unmap(vertex_buffer.Get(), 0);

	//頂点バッファーのバインド
	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	_immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//プリミティブタイプおよびデータの順序に関する情報のバインド
	_immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//入力レイアウトオブジェクトのバインド
	_immediate_context->IASetInputLayout(input_layout.Get());

	//プリミティブの描画
	_immediate_context->Draw(static_cast<UINT>(vertex_count), 0);

}

void SameSprite::Render()
{
	Render(BasePoint::LeftTop, 0, 0, Graphics::Instance().GetScreenWidth(), Graphics::Instance().GetScreenHeight(),
		0.0f, 0.0f,
		static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height),
		0.0f,
		{ 1.0f, 1.0f, 1.0f, 1.0f });
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy,
	float dw, float dh,
	float angle,
	DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, dw, dh, 0.0f, 0.0f,
		static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height),
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, size.x, size.y, 0.0f, 0.0f,
		static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height),
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, DirectX::XMFLOAT2 size,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, size.x, size.y, 0.0f, 0.0f,
		static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height),
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, float dw, float dh,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, dw, dh, 0.0f, 0.0f,
		static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height),
		angle,
		color);
}

void SameSprite::Render(
	BasePoint basePoint,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float angle,
	DirectX::XMFLOAT4 color)
{
	ID3D11DeviceContext* _immediate_context = Graphics::Instance().GetDeviceContext();

	//スクリーン(ビューポート)のサイズ取得
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	_immediate_context->RSGetViewports(&num_viewports, &viewport);

	//左上	//右上	//左下	//右下
	float x0, y0, x1, y1, x2, y2, x3, y3;

	//回転用の中心点
	DirectX::XMFLOAT2 center{};

	switch (basePoint)
	{
	case BasePoint::Center:
		x0 = dx - dw * 0.5f;
		y0 = dy - dh * 0.5f;
		x1 = dx + dw * 0.5f;
		y1 = dy - dh * 0.5f;
		x2 = dx - dw * 0.5f;
		y2 = dy + dh * 0.5f;
		x3 = dx + dw * 0.5f;
		y3 = dy + dh * 0.5f;
		center.x = dx;
		center.y = dy;
		break;
	case BasePoint::LeftTop:
		x0 = dx;
		y0 = dy;
		x1 = dx + dw;
		y1 = dy;
		x2 = dx;
		y2 = dy + dh;
		x3 = dx + dw;
		y3 = dy + dh;
		center.x = dx + dw * 0.5f;
		center.y = dy + dh * 0.5f;
		break;
	case BasePoint::RightTop:
		x0 = dx - dw;
		y0 = dy;
		x1 = dx;
		y1 = dy;
		x2 = dx - dw;
		y2 = dy + dh;
		x3 = dx;
		y3 = dy + dh;
		center.x = dx - dw * 0.5f;
		center.y = dy + dh * 0.5f;
		break;
	case BasePoint::LeftBottom:
		x0 = dx;
		y0 = dy - dh;
		x1 = dx + dw;
		y1 = dy - dh;
		x2 = dx;
		y2 = dy;
		x3 = dx + dw;
		y3 = dy;
		center.x = dx + dw * 0.5f;
		center.y = dy - dh * 0.5f;
		break;
	case BasePoint::RightBottom:
		x0 = dx - dw;
		y0 = dy - dh;
		x1 = dx;
		y1 = dy - dh;
		x2 = dx - dw;
		y2 = dy;
		x3 = dx;
		y3 = dy;
		center.x = dx - dw * 0.5f;
		center.y = dy - dh * 0.5f;
		break;
	case BasePoint::Top:
		x0 = dx - dw * 0.5f;
		y0 = dy;
		x1 = dx + dw * 0.5f;
		y1 = dy;
		x2 = dx - dw * 0.5f;
		y2 = dy + dh;
		x3 = dx + dw * 0.5f;
		y3 = dy + dh;
		center.x = dx;
		center.y = dy + dh * 0.5f;
		break;
	case BasePoint::Bottom:
		x0 = dx - dw * 0.5f;
		y0 = dy - dh;
		x1 = dx + dw * 0.5f;
		y1 = dy - dh;
		x2 = dx - dw * 0.5f;
		y2 = dy;
		x3 = dx + dw * 0.5f;
		y3 = dy;
		center.x = dx;
		center.y = dy - dh * 0.5f;
		break;
	case BasePoint::Left:
		x0 = dx;
		y0 = dy - dh * 0.5f;
		x1 = dx + dw;
		y1 = dy - dh * 0.5f;
		x2 = dx;
		y2 = dy + dh * 0.5f;
		x3 = dx + dw;
		y3 = dy + dh * 0.5f;
		center.x = dx + dw * 0.5f;
		center.y = dy;
		break;
	case BasePoint::Right:
		x0 = dx - dw;
		y0 = dy - dh * 0.5f;
		x1 = dx;
		y1 = dy - dh * 0.5f;
		x2 = dx - dw;
		y2 = dy + dh * 0.5f;
		x3 = dx;
		y3 = dy + dh * 0.5f;
		center.x = dx - dw * 0.5f;
		center.y = dy;
		break;
	}

	//回転処理
	Rotate(x0, y0, { center }, angle);
	Rotate(x1, y1, { center }, angle);
	Rotate(x2, y2, { center }, angle);
	Rotate(x3, y3, { center }, angle);


	//NDCへの変換
	x0 = 2.0f * x0 / viewport.Width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / viewport.Height;
	x1 = 2.0f * x1 / viewport.Width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / viewport.Height;
	x2 = 2.0f * x2 / viewport.Width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / viewport.Height;
	x3 = 2.0f * x3 / viewport.Width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / viewport.Height;

	float u0{ sx / texture2d_desc.Width };
	float v0{ sy / texture2d_desc.Height };
	float u1{ (sx + sw) / texture2d_desc.Width };
	float v1{ (sy + sh) / texture2d_desc.Height };

	vertices.push_back({ {x0,y0,0},{color.x,color.y,color.z,color.w},{u0,v0} });
	vertices.push_back({ {x1,y1,0},{color.x,color.y,color.z,color.w},{u1,v0} });
	vertices.push_back({ {x2,y2,0},{color.x,color.y,color.z,color.w},{u0,v1} });
	vertices.push_back({ {x2,y2,0},{color.x,color.y,color.z,color.w},{u0,v1} });
	vertices.push_back({ {x1,y1,0},{color.x,color.y,color.z,color.w},{u1,v0} });
	vertices.push_back({ {x3,y3,0},{color.x,color.y,color.z,color.w},{u1,v1} });
}

void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size, 
	DirectX::XMFLOAT2 cutPos, DirectX::XMFLOAT2 cutSize, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, size.x, size.y,
		cutPos.x, cutPos.y, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, DirectX::XMFLOAT2 size,
	DirectX::XMFLOAT2 cutPos, DirectX::XMFLOAT2 cutSize, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, size.x, size.y,
		cutPos.x, cutPos.y, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, float dw, float dh, 
	DirectX::XMFLOAT2 cutPos, DirectX::XMFLOAT2 cutSize,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, dw, dh,
		cutPos.x, cutPos.y, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,
	float sx, float sy,DirectX::XMFLOAT2 cutSize,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, size.x, size.y,
		sx, sy, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,
	DirectX::XMFLOAT2 cutPos, float sw, float sh, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, size.x, size.y,
		cutPos.x, cutPos.y, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, float dw, float dh,
	DirectX::XMFLOAT2 cutPos, DirectX::XMFLOAT2 cutSize, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, dw, dh,
		cutPos.x, cutPos.y, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, DirectX::XMFLOAT2 size,
	float sx, float sy, DirectX::XMFLOAT2 cutSize,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, size.x, size.y,
		sx, sy, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, DirectX::XMFLOAT2 size,
	DirectX::XMFLOAT2 cutPos, float sw, float sh, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, size.x, size.y,
		cutPos.x, cutPos.y, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, float dw, float dh,
	float sx, float sy, DirectX::XMFLOAT2 cutSize,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, dw, dh,
		sx, sy, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, float dw, float dh,
	DirectX::XMFLOAT2 cutPos, float sw, float sh,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, dw, dh,
		cutPos.x, cutPos.y, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size,
	float sx, float sy, float sw, float sh,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, size.x, size.y,
		sx, sy, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, float dw, float dh,
	float sx, float sy, DirectX::XMFLOAT2 cutSize, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, dw, dh,
		sx, sy, cutSize.x, cutSize.y,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, DirectX::XMFLOAT2 size,
	float sx, float sy, float sw, float sh,
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, size.x, size.y,
		sx, sy, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	float dx, float dy, float dw, float dh,
	DirectX::XMFLOAT2 cutPos, float sw, float sh, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, dx, dy, dw, dh,
		cutPos.x, cutPos.y, sw, sh,
		angle,
		color);
}
void SameSprite::Render(BasePoint basePoint,
	DirectX::XMFLOAT2 pos, float dw, float dh,
	float sx, float sy, float sw, float sh, 
	float angle, DirectX::XMFLOAT4 color)
{
	Render(basePoint, pos.x, pos.y, dw, dh,
		sx, sy, sw, sh,
		angle,
		color);
}

void SameSprite::Rotate(float& x, float& y, DirectX::XMFLOAT2 center, float angle)
{
	x -= center.x;
	y -= center.y;

	float cos = cosf(DirectX::XMConvertToRadians(angle));
	float sin = sinf(DirectX::XMConvertToRadians(angle));

	float tx{ x }, ty{ y };
	x = cos * tx + -sin * ty;
	y = sin * tx + cos * ty;

	x += center.x;
	y += center.y;
}
