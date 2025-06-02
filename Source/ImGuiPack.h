#pragma once
#include <windows.h>

#ifdef USE_IMGUI
#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_internal.h"
#include "../External/imgui/imgui_impl_dx11.h"
#include "../External/imgui/imgui_impl_win32.h"
//#include "../External/imgui/ImGuizmo.h"　<---必要なファイルごとにincludeすること
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern ImWchar glyphRangesJapanese[];　<---必要なファイルごとにincludeすること
#endif