#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include <memory>
#include <d3d11.h>
#include <d3d11_1.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <xaudio2.h>

#include <dxgidebug.h>
#include <mutex>

#include "RenderingState.h"

#ifdef ENABLE_DIRECT2D
#include <d2d1_1.h>
#include <DWrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#endif

#include <mutex>

// �O���t�B�b�N�X
class Graphics
{
public:
	Graphics(HWND hWnd, bool fullscreen);
	~Graphics();

	// �C���X�^���X�擾
	static Graphics& Instance() { return *instance; }

	//�E�B���h�E�Y�n���h��
	HWND GetHWND() const { return hWnd; }

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return device_context.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain1* GetSwapChain() const { return swap_chain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> getDepth() const { return depth_stencil_view; }

	//xaudio2�擾
	IXAudio2* GetXaudio2() const { return xaudio2.Get(); }

	//�����_�����O�X�e�[�g�擾
	RenderingState* GetRenderingState() { return rendering_state.get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return static_cast<float>(framebufferDimensions.cx); }

	// �X�N���[�������擾
	float GetScreenHeight() const { return static_cast<float>(framebufferDimensions.cy); }

	//�A�X�y�N�g��擾
	float GetAspectRatio() const { return aspectRatio; }

	float GetFeelingSize() const { return feelingSize; }

	//�t���X�N���[���̏󋵎擾
	bool IsFullscreen() { return fullscreen_mode; }

	//�~���[�e�b�N�X�擾
	std::mutex& GetMutex() { return mutex; }

	//�t���X�N���[���؂�ւ�
	void ChangeFullScreen(HWND hwnd, bool fullscreen);

	void OnSizeChanged(HWND hWnd, UINT64 width, UINT height);

	void CreateSwapChain(HWND hWnd, IDXGIFactory6* dxgi_factory6);

#ifdef ENABLE_DIRECT2D
	void CreateDirect2dObjects();

	ID2D1DeviceContext* GetD2dDeviceContext() { return d2d_device_context.Get(); }

	IDWriteFactory* GetDwriteFactory() { return dwrite_factory.Get(); }
	ID2D1RenderTarget* GetD2dRenderTarget() { return d2d_render_target.Get(); }
	ID2D1Factory* GetD2dFactory() { return d2d_factory.Get(); }
	ID2D1Bitmap1* GetBitMap() { return bitmap.Get(); }
	IDXGISurface* GetBackBuffer() { return back_buffer.Get(); }

	ID3D11Texture2D* GetSharedTexture() { return sharedTexture.Get(); }
#endif

private:
	static Graphics* instance;

	Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;
	SIZE framebufferDimensions;
	float aspectRatio;
	float feelingSize;
	bool tearing_supported{ false };

#ifdef ENABLE_DIRECT2D
	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgi_device;
	Microsoft::WRL::ComPtr<ID2D1Device> d2d_device;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d_device_context;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> d2d_render_target;
	Microsoft::WRL::ComPtr<ID2D1Factory1> d2d_factory;
	Microsoft::WRL::ComPtr<IDWriteFactory> dwrite_factory;
	Microsoft::WRL::ComPtr<IDXGISurface> back_buffer;
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> sharedTexture;
#endif
	const HWND hWnd = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;
	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory6;
	Microsoft::WRL::ComPtr<IXAudio2> xaudio2;

	std::unique_ptr<RenderingState> rendering_state;

	bool fullscreen_mode;
	RECT windowed_rect;
	DWORD windowed_style;

	std::mutex mutex;

	Microsoft::WRL::ComPtr<ID3D11Debug>mD3dDebug;
public:
	float fps = 0;
};

