#pragma once
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include "BasePoint.h"

class Sprite
{
public:
	Sprite(const wchar_t* filename, BasePoint c_basePoint = BasePoint::Center);
	~Sprite() {}

	void ReloadTexture(ID3D11Device* a_device,const wchar_t* filename);

	//画面サイズに引き伸ばして描画
	void Render();
	//描画位置＆描画サイズ、色、角度を指定して描画
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	//描画位置＆描画サイズ、切り取り位置、切り取りサイズ色、角度を指定して描画
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*座標*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*切り取り開始位置*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		float dx, float dy,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		float dw, float dh, /*矩形のサイズ*/
		float sx, float sy, /*切り取り開始位置*/
		float sw, float sh, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);

	void Render(DirectX::XMFLOAT2 a_basePoint,/*基準点*/
		DirectX::XMFLOAT2 pos,	/*座標*/
		DirectX::XMFLOAT2 size, /*矩形のサイズ*/
		DirectX::XMFLOAT2 cutPos,	/*切り取り開始位置*/
		DirectX::XMFLOAT2 cutSize, /*切り取りサイズ*/
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);

	void OutLineRender(BasePoint basePoint,/*基準点*/
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*色(rgba(0 ~ 1))*/);

	void OutLineRender(BasePoint basePoint,/*基準点*/
		float dx, float dy, float dw, float dh,
		float angle = 0.0f, /*90,180,270など*/
		DirectX::XMFLOAT4 color = { 0.0f,0.0f,0.0f,1.0f });

	// テクスチャ幅取得
	float GetTextureWidth() const { return textureWidth; }
	// テクスチャ高さ取得
	float GetTextureHeight() const { return textureHeight; }
	//アスペクト比取得
	float GetAspectRation() const { return aspectRation; }
	//ベースポイント取得
	BasePoint GetBasePoint() const { return base_point; }

	ID3D11ShaderResourceView* GetShaderResource() const { return shader_resource_view.Get(); }
private:
	//centerを中心にx,yをangle分回転させる関数
	void Rotate(float& x, float& y, DirectX::XMFLOAT2 center, float angle);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[2];
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	//頂点
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

	float textureWidth = 0.0f;
	float textureHeight = 0.0f;
	float aspectRation = 0.0f;
	BasePoint base_point = BasePoint::LeftTop;
};