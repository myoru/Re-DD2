#pragma once
#include <memory>
#include <iostream>
#include "Audio.h"
#include "Sprite.h"
#include "SameSprite.h"
#include "Shader.h"
#include "FullscreenQuad.h"
#include "ConstantBuffer.h"
#include "Framebuffer.h"
#include "ImGuiPack.h"

struct FadeConstants;

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	//初期化
	virtual void Initialize() = 0;
	//終了化
	virtual void Finalize() = 0;

#pragma region CommonScenes
	//共通項目初期化(static変数)
	void CommonInitialize();
	void CommonOnSizeChange();
#pragma endregion CommonScenes

	//更新処理
	virtual void Update(float elapsedTime) = 0;
	//描画処理
	virtual void Render(float elapsedTime) = 0;
#ifdef USE_IMGUI
	//ImGui描画関数
	virtual void ImGuiRender() = 0;
	virtual void CommonImGuiRender(DirectX::XMFLOAT2 a_position = { -1.0f,1.0f }, DirectX::XMFLOAT2 a_size = { -1.0f,1.0f }, ImGuiWindowFlags a_flags = ImGuiWindowFlags_None);
#endif // USE_IMGUI
	//RenderTargetのClear関数
	void ClearRenderTarget();
	//画面サイズ変更時に呼ばれる関数(仮想関数)
	virtual void OnSizeChange() {};

#pragma region Getter&Setter
	//ready
	bool IsReady() const { return m_ready; }
	void SetReady() { m_ready = true; }
	//index
	int GetIndex() const { return m_index; }
	void SetIndex(int a_index) { m_index = a_index; }
	std::string GetIndexStr();
#pragma endregion Getter&Setter

protected:
#pragma region CommonScenes
	//ファイナルパス描画用変数
	static std::unique_ptr<FullscreenQuad> m_bitBlockTransfer;
	//FrameBuffer系
	static std::unique_ptr<FrameBuffer> m_fb_fade ; //フェードイン・フェードアウト用のフレームバッファ
	//PixelShader(ピクセルシェーダー)
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_fb_ps_fade; //フェードイン・フェードアウト用のピクセルシェーダー
	//定数バッファ
	static std::unique_ptr<ConstantBuffer<FadeConstants>> m_cb_fade; //フェードイン・フェードアウト用の定数バッファ
	//初期化したかを確認する変数
	static bool m_initializeChecker;
	//Clearカラー
	static DirectX::XMFLOAT4 m_clearColor;
	//背景色
	static DirectX::XMFLOAT4 m_backColor;
	//画面サイズ
	static DirectX::XMFLOAT2 screenSize;
#pragma endregion CommonScenes
	//BGM・SE
	std::vector<std::unique_ptr<Audio>> m_seBox;
	std::vector<std::unique_ptr<Audio>> m_bgmBox;
	//Sprite(スプライト)(各派生クラスごとのenum classで定義する 例.SceneTitle::SpriteKind::TitkeLogo)
	std::vector<std::unique_ptr<Sprite>> m_spriteBox;
	std::vector<std::unique_ptr<SameSprite>> m_sameSpriteBox;
private:
	//Scene識別インデックス
	int m_index = 0;
protected:
	//準備完了フラグ
	bool m_ready = false;
public:
	enum class Index
	{
		Test,
		Loading,
		Title,
		Game,
		Tool,
		Event,
	};
};

//フェードイン・フェードアウト用の構造体(定数バッファで使う)
struct FadeConstants
{
	//初期化処理
	void Initialize(float time, float fadeValue, int inOrOut, int isStart, int blackOrWhite = -1);
	//更新処理
	bool Update(float elapsedTime);

	float m_time = 0.5f;       // フェードにかかる時間
	float m_fadeValue = 0.0f; // 現在のフェード値
	int m_inOutFlag = 1;       // 1: in, 0: out
	int m_isStart = 0;           // 1: 開始, 0: 未開始
	int m_blackWhite = 0;	 // 1: Black, 0: White

	enum class Fade
	{
		Out,
		In,
	};
	enum class BlackWhite
	{
		Clear = -1,
		Black = 0,
		White = 1,
	};
};