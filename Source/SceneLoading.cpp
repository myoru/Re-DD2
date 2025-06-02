#include "Graphics.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneLoading.h"

using namespace DirectX;
template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

//初期化
void SceneLoading::Initialize()
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();

	//Scene識別インデックス設定
	Scene::SetIndex(sc_i(Scene::Index::Loading));

	CommonInitialize();

	white_spr = std::make_unique<SameSprite>(nullptr, BasePoint::LeftTop);
	/*hack_logo_spr = std::make_unique<SameSprite>(L".\\Data\\Sprite\\HackLogo2.png", BasePoint::Center);
	if (draw_explanation)
	{
		explanation_spr = std::make_unique<SameSprite>(L".\\Data\\Sprite\\HackLogo2.png", BasePoint::Center);
	}*/
	//スレッドの開始
	thread = new std::thread(LoadingThread, this);
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了化
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
	/*if (hack_logo_spr_alpha_trg)
	{
		hack_logo_spr_alpha += elapsedTime * 1.2f;
		if (hack_logo_spr_alpha >= 1.3f)
		{
			hack_logo_spr_alpha = 1.0f;
			hack_logo_spr_alpha_trg = false;
		}
	}
	else
	{
		hack_logo_spr_alpha -= elapsedTime * 1.5f;
		if (hack_logo_spr_alpha <= 0.0f)
		{
			hack_logo_spr_alpha = 0.0f;
			hack_logo_spr_alpha_trg = true;
		}
	}*/

	//次のシーンの準備が完了したらシーンを切り替える
	if (next_scene->IsReady())SceneManager::Instance().ChangeScene(next_scene);
}

//描画処理
void SceneLoading::Render(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	RenderingState* _rendering_state = _graphics.GetRenderingState();

	//画面サイズ
	XMFLOAT2 _screen_size = { _graphics.GetScreenWidth(),_graphics.GetScreenHeight() };

	//RenderTargetのClear
	ClearRenderTarget();

	//2Dスプライト描画
	{
		_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
		_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
		_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);

		white_spr->Begin();
		white_spr->Render(white_spr->GetBasePoint(), 0.0f, 0.0f, _graphics.GetScreenWidth(), _graphics.GetScreenHeight(), 0.0f, { 0.0f,0.0f,0.0f,1.0f });
		white_spr->End();

		/*hack_logo_spr->Begin();
		hack_logo_spr->Render(hack_logo_spr->GetBasePoint(), _screen_size.x - (_screen_size.x * 0.15f / 2), _screen_size.y - (_screen_size.x * 0.15f / 2), _screen_size.x * 0.1f, _screen_size.x * 0.1f, 0.0f, { 0.8666666f,0.8666666f,0.8666666f,hack_logo_spr_alpha });
		hack_logo_spr->End();*/
	}
}

#ifdef USE_IMGUI
void SceneLoading::ImGuiRender()
{
}
#endif // USE_IMGUI

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	//次のシーンの初期化を行う
	scene->next_scene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->next_scene->SetReady();
}