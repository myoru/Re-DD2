#include "Framework.h"

//Scene系
#include "SceneLoading.h"
#include "SceneTool.h"

Framework::Framework(HWND hWnd, bool fullscreen) : hWnd(hWnd), graphics(hWnd, fullscreen), input(hWnd)
{
	//どのシーンからスタートするかをココで決める
	//m_sceneManager.Initialize();
	m_sceneManager.ChangeScene(new SceneLoading(new SceneTool())); //タイトルからスタート
}

Framework::~Framework()
{
	m_sceneManager.Clear();
}

int Framework::run()
{
	{
		ID3D11Device* _device = graphics.GetDevice();
		ID3D11DeviceContext* _immediate_context = graphics.GetDeviceContext();
		IDXGISwapChain1* _swap_chain = graphics.GetSwapChain();

		MSG msg{};

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImFont* jpFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 20.0f, nullptr,
			ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
		ImGui::GetIO().FontDefault = jpFont;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(_device, _immediate_context);
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				CalculateFrameStats();

#ifdef USE_IMGUI
				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				ImVec2 pos = ImGui::GetMainViewport()->GetWorkPos();
				ImVec2 size = ImGui::GetIO().DisplaySize;
				ImGuizmo::BeginFrame();
#if 0
				ImGuizmo::SetOrthographic(false);
#else
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);
#endif

#endif
				Update(tictoc.time_interval());
				Render(tictoc.time_interval());

#ifdef USE_IMGUI
				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
				if (_swap_chain)
				{
					//0で上限なし 1で60固定
					UINT sync_interval{ 0 };
					//UINT sync_interval{ 1 };
					_swap_chain->Present(sync_interval, 0);
				}
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif

		BOOL fullscreen = 0;
		_swap_chain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			_swap_chain->SetFullscreenState(FALSE, 0);
		}

		return static_cast<int>(msg.wParam);
	}
}

void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
	input.Update();

	//ここでsceneのupdate()を呼ぶ！
	m_sceneManager.Update(elapsedTime);
}

void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
	std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());

	HRESULT hr{ S_OK };

	ID3D11DeviceContext* _immediate_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* _render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* _depth_stencil_view = graphics.GetDepthStencilView();

	//この処理が必要かは質問する
	{
		ID3D11RenderTargetView* null_render_target_views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT]{};
		_immediate_context->OMSetRenderTargets(_countof(null_render_target_views), null_render_target_views, 0);
		ID3D11ShaderResourceView* null_shader_resource_views[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT]{};
		_immediate_context->VSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
		_immediate_context->PSSetShaderResources(0, _countof(null_shader_resource_views), null_shader_resource_views);
	}

	_immediate_context->ClearDepthStencilView(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_immediate_context->OMSetRenderTargets(1, &_render_target_view, _depth_stencil_view);

	//ここでrender()を呼ぶ！
	m_sceneManager.Render(elapsedTime);
#ifdef USE_IMGUI
	ImGuiStyle& _imGuiStyle = ImGui::GetStyle();
	/*_imGuiStyle.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.5f, 1.0f);
	_imGuiStyle.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.5f, 0.3f, 1.0f);*/
	_imGuiStyle.Colors[ImGuiCol_Border] = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
	_imGuiStyle.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	_imGuiStyle.WindowRounding = 0.1f;
	//ImGui::Begin("ImGUI");
	m_sceneManager.ImGuiRender();
	//ImGui::End();
#endif // USE_IMGUI
}
