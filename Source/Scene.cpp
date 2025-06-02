#include "Scene.h"
#include "Graphics.h"

//Graphics& _graphics = Graphics::Instance();
//ID3D11Device* _device = _graphics.GetDevice();

std::unique_ptr<FullscreenQuad> Scene::m_bitBlockTransfer = nullptr;
std::unique_ptr<FrameBuffer> Scene::m_fb_fade = nullptr;
Microsoft::WRL::ComPtr<ID3D11PixelShader> Scene::m_fb_ps_fade = nullptr;
std::unique_ptr<ConstantBuffer<FadeConstants>> Scene::m_cb_fade = nullptr;
bool Scene::m_initializeChecker = false;
DirectX::XMFLOAT4 Scene::m_clearColor = { 0.0f,1.0f,0.7f,1.0f };
DirectX::XMFLOAT4 Scene::m_backColor = { 0.0f,0.0f,0.0f,1.0f };

template <typename T>
constexpr int sc_i(T value)
{
	return static_cast<int>(value);
}

//‰Šú‰»ˆ—
void FadeConstants::Initialize(float time, float fadeValue, int inOrOut, int isStart, int blackOrWhite)
{
	this->m_time = time;
	this->m_fadeValue = fadeValue;
	this->m_inOutFlag = inOrOut;
	this->m_isStart = isStart;
	if (blackOrWhite >= 0)
	{
		this->m_blackWhite = blackOrWhite;
	}
}
//XVˆ—
bool FadeConstants::Update(float elapsedTime)
{
	if (m_isStart)
	{
		if (m_inOutFlag)
		{
			m_fadeValue += (elapsedTime / m_time);
			if (m_fadeValue >= 1.0f)
			{
				m_fadeValue = 1.0f;
				return true;
			}
		}
		else
		{
			m_fadeValue -= (elapsedTime / m_time);
			if (m_fadeValue <= 0.0f)
			{
				m_fadeValue = 0.0f;
				return true;
			}
		}
	}
	return false;
}

void Scene::CommonInitialize()
{
	//‚Ü‚¾‰Šú‰»‚ªÏ‚ñ‚Å‚È‚¯‚ê‚Î‰Šú‰»‚·‚é
	if (!m_initializeChecker)
	{
		Graphics& _graphics = Graphics::Instance();
		ID3D11Device* _device = _graphics.GetDevice();

		m_bitBlockTransfer = std::make_unique<FullscreenQuad>(_device);
		m_fb_fade = std::make_unique<FrameBuffer>(_device, _graphics.GetScreenWidth(), _graphics.GetScreenHeight());
		CreatePsFromCso(_device, ".\\Data\\Shader\\black_fade_out_ps.cso", m_fb_ps_fade.GetAddressOf());
		m_cb_fade = std::make_unique<ConstantBuffer<FadeConstants>>(_device);
		m_initializeChecker = true;
	}
}

void Scene::CommonOnSizeChange()
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11Device* _device = _graphics.GetDevice();

	m_fb_fade = std::make_unique<FrameBuffer>(_device, _graphics.GetScreenWidth(), _graphics.GetScreenHeight());
}

#ifdef USE_IMGUI
void Scene::CommonImGuiRender(DirectX::XMFLOAT2 a_position, DirectX::XMFLOAT2 a_size, ImGuiWindowFlags a_flags)
{
	if (a_position.x >= 0.0f)
	{
		ImGui::SetNextWindowPos(ImVec2(a_position.x, a_position.y), ImGuiCond_Always);
	}
	if (a_size.x >= 0.0f)
	{
		ImGui::SetNextWindowSize(ImVec2(a_size.x, a_size.y), ImGuiCond_Always);
	}
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
	ImGui::Begin("Common Elements",nullptr, a_flags);
	if (m_index == sc_i(Index::Tool))return;
	std::string _index_str = "Scene : ";
	ImGui::Text((_index_str + GetIndexStr()).c_str());
	if (ImGui::CollapsingHeader("Scene Common Element", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::RadioButton("Clear", &m_cb_fade->data.m_blackWhite, sc_i(FadeConstants::BlackWhite::Clear));		ImGui::SameLine();
		ImGui::RadioButton("Black", &m_cb_fade->data.m_blackWhite, sc_i(FadeConstants::BlackWhite::Black));		ImGui::SameLine();
		ImGui::RadioButton("White", &m_cb_fade->data.m_blackWhite, sc_i(FadeConstants::BlackWhite::White));
		if (m_cb_fade->data.m_blackWhite < 0)
		{
			ImGui::ColorEdit4("clear_color", &m_clearColor.x);
		}
		ImGui::ColorEdit4("back_color", &m_backColor.x);
		ImGui::SliderFloat("Fade", &m_cb_fade->data.m_fadeValue, 0.0f, 1.0f);
		ImGui::Separator();
	}
	ImGui::End();
	ImGui::PopStyleColor(2);
}
#endif // USE_IMGUI

void Scene::ClearRenderTarget()
{
	Graphics& _graphics = Graphics::Instance();
	ID3D11DeviceContext* _immediate_context = _graphics.GetDeviceContext();
	ID3D11RenderTargetView* _render_target_view = _graphics.GetRenderTargetView();
	ID3D11DepthStencilView* _depth_stencil_view = _graphics.GetDepthStencilView();

	FLOAT _clear_color[4]{};

	switch (static_cast<FadeConstants::BlackWhite>(Scene::m_cb_fade->data.m_blackWhite))
	{
	case FadeConstants::BlackWhite::Black:
		_clear_color[0] = _clear_color[1] = _clear_color[2] = 0.0f;
		break;
	case FadeConstants::BlackWhite::White:
		_clear_color[0] = _clear_color[1] = _clear_color[2] = 1.0f;
		break;
	default:
		_clear_color[0] = m_clearColor.x; _clear_color[1] = m_clearColor.y; _clear_color[2] = m_clearColor.z; _clear_color[3] = m_clearColor.w;
		break;
	}

	//RenderTarget‚ðClear
	_immediate_context->ClearRenderTargetView(_render_target_view, _clear_color);
	_immediate_context->ClearDepthStencilView(_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	_immediate_context->OMSetRenderTargets(1, &_render_target_view, _depth_stencil_view);
}

std::string Scene::GetIndexStr()
{
	switch (static_cast<Index>(m_index))
	{
	case Index::Test:
		return "Test";
	case Index::Loading:
		return "Loading";
	case Index::Title:
		return "Title";
	case Index::Game:
		return "Game";
	case Index::Tool:
		return "Tool";
	case Index::Event:
		return "Event";
	default:
		return "None";
	}
}
