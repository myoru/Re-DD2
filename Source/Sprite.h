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

	//��ʃT�C�Y�Ɉ����L�΂��ĕ`��
	void Render();
	//�`��ʒu���`��T�C�Y�A�F�A�p�x���w�肵�ĕ`��
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	//�`��ʒu���`��T�C�Y�A�؂���ʒu�A�؂���T�C�Y�F�A�p�x���w�肵�ĕ`��
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*���W*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		float dx, float dy,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);
	void Render(BasePoint basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		float dw, float dh, /*��`�̃T�C�Y*/
		float sx, float sy, /*�؂���J�n�ʒu*/
		float sw, float sh, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);

	void Render(DirectX::XMFLOAT2 a_basePoint,/*��_*/
		DirectX::XMFLOAT2 pos,	/*���W*/
		DirectX::XMFLOAT2 size, /*��`�̃T�C�Y*/
		DirectX::XMFLOAT2 cutPos,	/*�؂���J�n�ʒu*/
		DirectX::XMFLOAT2 cutSize, /*�؂���T�C�Y*/
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);

	void OutLineRender(BasePoint basePoint,/*��_*/
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }/*�F(rgba(0 ~ 1))*/);

	void OutLineRender(BasePoint basePoint,/*��_*/
		float dx, float dy, float dw, float dh,
		float angle = 0.0f, /*90,180,270�Ȃ�*/
		DirectX::XMFLOAT4 color = { 0.0f,0.0f,0.0f,1.0f });

	// �e�N�X�`�����擾
	float GetTextureWidth() const { return textureWidth; }
	// �e�N�X�`�������擾
	float GetTextureHeight() const { return textureHeight; }
	//�A�X�y�N�g��擾
	float GetAspectRation() const { return aspectRation; }
	//�x�[�X�|�C���g�擾
	BasePoint GetBasePoint() const { return base_point; }

	ID3D11ShaderResourceView* GetShaderResource() const { return shader_resource_view.Get(); }
private:
	//center�𒆐S��x,y��angle����]������֐�
	void Rotate(float& x, float& y, DirectX::XMFLOAT2 center, float angle);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[2];
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	//���_
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