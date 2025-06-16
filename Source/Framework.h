#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>

#include "Misc.h"
#include "HighResolutionTimer.h"

#include "Graphics.h"
#include "Input.h"
#include "ImGuiPack.h"
#include "AllFacialSet.h"

#ifdef USE_IMGUI
#include "../External/imgui/ImGuizmo.h"
extern ImWchar glyphRangesJapanese[];
#endif

CONST LPCWSTR APPLICATION_NAME{ L"SignBoard Create Tool" };

class Framework
{
public:
	const HWND hWnd;
	Graphics graphics;
	Input input;
	high_resolution_timer tictoc;

	Microsoft::WRL::ComPtr<IDXGIAdapter3> adapter;

	size_t VideoMemoryUsage()
	{
		DXGI_QUERY_VIDEO_MEMORY_INFO video_memory_info;
		adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &video_memory_info);
		return video_memory_info.CurrentUsage / 1024 / 1024;
	}

#ifdef ENABLE_DIRECT2D
	//Microsoft::WRL::ComPtr<ID2D1DeviceContext> d2d1_device_context;
	//Microsoft::WRL::ComPtr<IDWriteTextFormat> dwrite_text_formats[8];
	//Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> d2d_solid_color_brushes[8];
	//void create_direct2d_objects();
#endif
	Framework(HWND hWnd, bool fullscreen);
	~Framework();

	int run();

	//こいつは必要！！！
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN://ここの処理でESCを押すとウィンドウが落ちる
//#if _DEBUG
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
//#endif
		case WM_MOUSEWHEEL:
			Input::Instance().GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wparam));
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.Stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.Start();
			break;
		case WM_SIZE:
		{
#if 1
			RECT client_rect{};
			GetClientRect(hwnd, &client_rect);
			graphics.OnSizeChanged(hWnd, static_cast<UINT64>(client_rect.right - client_rect.left), client_rect.bottom - client_rect.top);
#endif
			break;
		}
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:

	//こいつは必要！！！
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Render(float elapsedTime/*Elapsed seconds from last frame*/);

private:
	//こいつは必要！！！
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void CalculateFrameStats()
	{
		if (++frames, (tictoc.TimeStamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);
			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hWnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};

