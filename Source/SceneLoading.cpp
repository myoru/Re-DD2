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

	m_spriteBox.emplace_back(std::make_unique<Sprite>(nullptr, BasePoint::LeftTop));
	m_spriteBox.emplace_back(std::make_unique<Sprite>(L".\\Data\\Sprite\\HackLogo2.png"));

	//スレッドの開始
	m_thread = new std::thread(LoadingThread, this);
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了化
	if (m_thread != nullptr)
	{
		m_thread->join();
		delete m_thread;
		m_thread = nullptr;
	}
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{
	if (m_logoSprAlphaTrg)
	{
		m_logoSprAlpha += elapsedTime * 1.2f;
		if (m_logoSprAlpha >= 1.3f)
		{
			m_logoSprAlpha = 1.0f;
			m_logoSprAlphaTrg = false;
		}
	}
	else
	{
		m_logoSprAlpha -= elapsedTime * 1.5f;
		if (m_logoSprAlpha <= 0.0f)
		{
			m_logoSprAlpha = 0.0f;
			m_logoSprAlphaTrg = true;
		}
	}

	//次のシーンの準備が完了したらシーンを切り替える
	if (m_nextScene->IsReady())
		SceneManager::GetInstance()->ChangeScene(m_nextScene);
}

//描画処理
void SceneLoading::Render(float elapsedTime)
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	RenderingState* _rendering_state = _graphics.GetRenderingState();

	//RenderTargetのClear
	ClearRenderTarget();

	//2Dスプライト描画
	{
		_rendering_state->SetBlendState(_immediate_context, BLEND_STATE::ALPHA);
		_rendering_state->SetDepthStencilState(_immediate_context, DEPTH_STENCIL_STATE::ZT_OFF_ZW_OFF);
		_rendering_state->SetRasterizerState(_immediate_context, RASTERIZER_STATE::CULL_NONE);

		float _logoSize = screenSize.y / 8.0f;
		m_spriteBox[sc_i(SpriteKind::Logo)]->Render(BasePoint::RightBottom, screenSize.x, screenSize.y, { _logoSize ,_logoSize }, 0.0f, { 0.8666666f,0.8666666f,0.8666666f,m_logoSprAlpha > 1.0f ? 1.0f : m_logoSprAlpha });
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

	SceneManager::GetInstance()->Initialize();

	//次のシーンの初期化を行う
	scene->m_nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->m_nextScene->SetReady();
}