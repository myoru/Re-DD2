#include "Misc.h"
#include "Graphics.h"
#include "SceneManager.h"

Graphics* Graphics::instance = nullptr;

void AcquireHighPerformanceAdapter(IDXGIFactory6* dxgiFactory6, IDXGIAdapter3** dxgiAdapter3)
{
	HRESULT hr{ S_OK };

	Microsoft::WRL::ComPtr<IDXGIAdapter3> enumerated_adapter;
	for (UINT adapter_index = 0; DXGI_ERROR_NOT_FOUND != dxgiFactory6->EnumAdapterByGpuPreference(adapter_index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(enumerated_adapter.ReleaseAndGetAddressOf())); ++adapter_index)
	{
		DXGI_ADAPTER_DESC1 adapter_desc;
		hr = enumerated_adapter->GetDesc1(&adapter_desc);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		if (adapter_desc.VendorId == 0x1002/*AMD*/ || adapter_desc.VendorId == 0x10DE/*NVIDIA*/)
		{
			OutputDebugStringW((std::wstring(adapter_desc.Description) + L" has been selected.\n").c_str());
			OutputDebugStringA(std::string("\tVendorId:" + std::to_string(adapter_desc.VendorId) + '\n').c_str());
			OutputDebugStringA(std::string("\tDeviceId:" + std::to_string(adapter_desc.DeviceId) + '\n').c_str());
			OutputDebugStringA(std::string("\tSubSysId:" + std::to_string(adapter_desc.SubSysId) + '\n').c_str());
			OutputDebugStringA(std::string("\tRevision:" + std::to_string(adapter_desc.Revision) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedVideoMemory:" + std::to_string(adapter_desc.DedicatedVideoMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tDedicatedSystemMemory:" + std::to_string(adapter_desc.DedicatedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tSharedSystemMemory:" + std::to_string(adapter_desc.SharedSystemMemory) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.HighPart:" + std::to_string(adapter_desc.AdapterLuid.HighPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tAdapterLuid.LowPart:" + std::to_string(adapter_desc.AdapterLuid.LowPart) + '\n').c_str());
			OutputDebugStringA(std::string("\tFlags:" + std::to_string(adapter_desc.Flags) + '\n').c_str());
			break;
		}
	}
	*dxgiAdapter3 = enumerated_adapter.Detach();
}

//この関数とコンストラクタを合わせる
void Graphics::CreateSwapChain(HWND hWnd, IDXGIFactory6* dxgi_factory6)
{
	HRESULT hr{ S_OK };

	if (swap_chain)
	{
		ID3D11RenderTargetView* null_render_target_view{};
		device_context->OMSetRenderTargets(1, &null_render_target_view, NULL);
		render_target_view.Reset();
#if 0
		immediate_context->Flush();
		immediate_context->ClearState();
#endif

		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain->GetDesc(&swap_chain_desc);

		hr = swap_chain->ResizeBuffers(swap_chain_desc.BufferCount, framebufferDimensions.cx, framebufferDimensions.cy, swap_chain_desc.BufferDesc.Format, swap_chain_desc.Flags);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
		hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		D3D11_TEXTURE2D_DESC texture2d_desc;
		render_target_buffer->GetDesc(&texture2d_desc);

		hr = device->CreateRenderTargetView(render_target_buffer.Get(), NULL, render_target_view.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	else
	{
		BOOL allow_tearing = FALSE;
		if (SUCCEEDED(hr))
		{
			hr = dxgi_factory6->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(allow_tearing));
		}
		tearing_supported = SUCCEEDED(hr) && allow_tearing;

		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc1{};
		swap_chain_desc1.Width = framebufferDimensions.cx;
		swap_chain_desc1.Height = framebufferDimensions.cy;
		swap_chain_desc1.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swap_chain_desc1.Stereo = FALSE;
		swap_chain_desc1.SampleDesc.Count = 1;
		swap_chain_desc1.SampleDesc.Quality = 0;
		swap_chain_desc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc1.BufferCount = 2;
		swap_chain_desc1.Scaling = DXGI_SCALING_STRETCH;
		swap_chain_desc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swap_chain_desc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swap_chain_desc1.Flags = tearing_supported ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
		hr = dxgi_factory6->CreateSwapChainForHwnd(device.Get(), hWnd, &swap_chain_desc1, nullptr, nullptr, swap_chain.ReleaseAndGetAddressOf());
#if 0
		swap_chain_desc1.Flags |= DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
#endif
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//alt + Enter でフルスクリーンにならないようにする。
		hr = dxgi_factory6->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		Microsoft::WRL::ComPtr<ID3D11Texture2D> render_target_buffer;
		hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(render_target_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		hr = device->CreateRenderTargetView(render_target_buffer.Get(), nullptr, render_target_view.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	//深度ステンシルビューの作成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
	D3D11_TEXTURE2D_DESC texture2d_desc{};
	texture2d_desc.Width = framebufferDimensions.cx;
	texture2d_desc.Height = framebufferDimensions.cy;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2d_desc, nullptr, depth_stencil_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, depth_stencil_view.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//ビューポート設定
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(framebufferDimensions.cx);
	viewport.Height = static_cast<float>(framebufferDimensions.cy);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	device_context->RSSetViewports(1, &viewport);
}

#ifdef ENABLE_DIRECT2D
void Graphics::CreateDirect2dObjects()
{
	HRESULT hr{ S_OK };

	D2D1_FACTORY_OPTIONS factory_options{};
#ifdef _DEBUG
	factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif //_DEBUG

#ifdef ENABLE_DIRECT2D
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory_options, d2d_factory.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	

	/*D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		0, 0);*/

	//hr = d2d_factory->CreateDxgiSurfaceRenderTarget(back_buffer.Get(), &props, d2d_render_target.ReleaseAndGetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(dwrite_factory.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	device.As(&dxgi_device);
	dxgi_device->SetMaximumFrameLatency(1);

	hr = device->QueryInterface(IID_PPV_ARGS(dxgi_device.ReleaseAndGetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = d2d_factory->CreateDevice(dxgi_device.Get(), d2d_device.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d_device_context.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	auto prop = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));

	hr = d2d_device_context->CreateBitmapFromDxgiSurface(back_buffer.Get(), &prop, bitmap.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	d2d_device_context->SetTarget(bitmap.Get());

#endif //ENABLE_DIRECT2D

#ifdef ENABLE_DIRECT2D

	//hr = dwrite_factory->CreateTextFormat(L"Meiryo",
	//	NULL,
	//	DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL,
	//	DWRITE_FONT_STRETCH_NORMAL,
	//	11,
	//	L"",
	//	text_format[0].ReleaseAndGetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//hr = text_format[0]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	////フォント設定
	//hr = dwrite_factory->CreateTextFormat(L"Impact",
	//	NULL,
	//	DWRITE_FONT_WEIGHT_NORMAL,
	//	DWRITE_FONT_STYLE_NORMAL,
	//	DWRITE_FONT_STRETCH_NORMAL,
	//	24,
	//	L"",
	//	text_format[1].ReleaseAndGetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//hr = text_format[1]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	///*dwrite_factory->CreateTextFormat(
	//	L"Data\\Sprite\\Fonts\\851letrogo.ttf",
	//	NULL,
	//	DWRITE_FONT_WEIGHT_REGULAR,
	//	DWRITE_FONT_STYLE_NORMAL,
	//	DWRITE_FONT_STRETCH_NORMAL,
	//	32.0f,
	//	L"ja-jp",
	//	text_format[2].ReleaseAndGetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	//hr = text_format[2]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));*/

	////ブラシ設定
	//hr = d2d_device_context->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), brush.ReleaseAndGetAddressOf());
	//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#endif//ENABLE_DIRECT2D
}
#endif

// コンストラクタ
Graphics::Graphics(HWND hWnd, bool fullscreen)
	:hWnd(hWnd)
	, fullscreen_mode(fullscreen)
	, windowed_style(static_cast<DWORD>(GetWindowLongPtrW(hWnd, GWL_STYLE)))//要調査
{
	//フルスクリーンの設定をする(コンストラクタはGraphicsでアップデートもGraphicsで)
	if (fullscreen)
	{
		ChangeFullScreen(hWnd, true);
	}

	RECT client_rect;
	GetClientRect(hWnd, &client_rect);
	framebufferDimensions.cx = client_rect.right - client_rect.left;
	framebufferDimensions.cy = client_rect.bottom - client_rect.top;

	aspectRatio = static_cast<float>(framebufferDimensions.cy) / static_cast<float>(framebufferDimensions.cx);

	feelingSize = static_cast<float>(framebufferDimensions.cx) / 1920.0f;

	//インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	HRESULT hr = S_OK;

	// デバイス＆スワップチェーンの生成
	UINT create_factory_flags{};
#ifdef _DEBUG
	create_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	hr = CreateDXGIFactory2(create_factory_flags, IID_PPV_ARGS(dxgi_factory6.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	AcquireHighPerformanceAdapter(dxgi_factory6.Get(), adapter.GetAddressOf());

	UINT create_device_flags = 0;
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#ifdef ENABLE_DIRECT2D
	create_device_flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	//D3D_DRIVER_TYPE driverTypes[] =
	//{
	//	D3D_DRIVER_TYPE_HARDWARE,
	//	D3D_DRIVER_TYPE_WARP,
	//	D3D_DRIVER_TYPE_REFERENCE,
	//};
	//UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL feature_levels[] =
	{

		D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	/*D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,*/
	};

	/*for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driver_type = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(adapter.Get(), driver_type, nullptr, create_device_flags, feature_levels, _countof(feature_levels),
			D3D11_SDK_VERSION, device.ReleaseAndGetAddressOf(), &feature_level, device_context.ReleaseAndGetAddressOf());

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(adapter.Get(), driver_type, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}*/


	D3D_FEATURE_LEVEL feature_level;
	hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr,
		create_device_flags, feature_levels, _countof(feature_levels), D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(), &feature_level, device_context.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	_ASSERT_EXPR(!(feature_level < D3D_FEATURE_LEVEL_11_0), L"Direct3D Feature Level 11 unsupported.");

	CreateSwapChain(hWnd, dxgi_factory6.Get());
#ifdef ENABLE_DIRECT2D
	CreateDirect2dObjects();
#endif

	//xaudio2
	hr = XAudio2Create(xaudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rendering_state = std::make_unique<RenderingState>(device.Get());
	rendering_state->set_sampler_states(device_context.Get());

	IXAudio2MasteringVoice* _master_voice = nullptr;
	hr = xaudio2->CreateMasteringVoice(&_master_voice);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

// デストラクタ
Graphics::~Graphics()
{
}

void Graphics::ChangeFullScreen(HWND hwnd, bool fullscreen)
{
	fullscreen_mode = fullscreen;
	if (fullscreen)
	{
		GetWindowRect(hwnd, &windowed_rect);
		SetWindowLongPtrA(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

		RECT fullscreen_window_rect;

		HRESULT hr{ E_FAIL };
		if (swap_chain)
		{
			Microsoft::WRL::ComPtr<IDXGIOutput> dxgi_output;
			hr = swap_chain->GetContainingOutput(&dxgi_output);
			if (hr == S_OK)
			{
				DXGI_OUTPUT_DESC output_desc;
				hr = dxgi_output->GetDesc(&output_desc);
				if (hr == S_OK)
				{
					fullscreen_window_rect = output_desc.DesktopCoordinates;
				}
			}
		}
		if (hr != S_OK)
		{
			DEVMODE devmode = {};
			devmode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

			fullscreen_window_rect = {
				devmode.dmPosition.x,
				devmode.dmPosition.y,
				devmode.dmPosition.x + static_cast<LONG>(devmode.dmPelsWidth),
				devmode.dmPosition.y + static_cast<LONG>(devmode.dmPelsHeight)
			};
		}
		SetWindowPos(
			hwnd,
			NULL,
			fullscreen_window_rect.left,
			fullscreen_window_rect.top,
			fullscreen_window_rect.right,
			fullscreen_window_rect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hwnd, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLongPtrA(hwnd, GWL_STYLE, windowed_style);
		SetWindowPos(
			hwnd,
			HWND_NOTOPMOST,
			windowed_rect.left,
			windowed_rect.top,
			windowed_rect.right - windowed_rect.left,
			windowed_rect.bottom - windowed_rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(hwnd, SW_NORMAL);
	}
}

void Graphics::OnSizeChanged(HWND hWnd, UINT64 width, UINT height)
{
	HRESULT hr{ S_OK };
	if (width != framebufferDimensions.cx || height != framebufferDimensions.cy)
	{
		framebufferDimensions.cx = static_cast<LONG>(width);
		framebufferDimensions.cy = static_cast<LONG>(height);

		feelingSize = static_cast<float>(framebufferDimensions.cx) / 1920.0f;

		if (framebufferDimensions.cx == 0 && framebufferDimensions.cy == 0)
		{
			return;
		}

		//aspect_ratio = static_cast<float>(framebuffer_dimensions.cx) / static_cast<float>(framebuffer_dimensions.cy);

		// Release all objects that hold shader resource views here.

		SceneManager::Instance().OnSizeChange();


#ifdef ENABLE_DIRECT2D
		//d2d_device_context.Reset();
#endif

		Microsoft::WRL::ComPtr<IDXGIFactory6> dxgi_factory6;
		hr = swap_chain->GetParent(IID_PPV_ARGS(dxgi_factory6.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		CreateSwapChain(hWnd, dxgi_factory6.Get());

#ifdef ENABLE_DIRECT2D
		CreateDirect2dObjects();
#endif


	}
}