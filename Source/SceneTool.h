#pragma once
#include "Scene.h"
#include "SignBoard.h"
#include "Chapter.h"
#include "RectUI.h"
#include <unordered_map>


class SceneTool : public Scene
{
public:
	SceneTool() {};
	~SceneTool() override {}

	//初期化
	void Initialize() override;
	//終了化
	void Finalize()override;
	//更新処理
	void Update(float elapsedTime)override;
	void CharactersUpdate(float a_elapsedTime);
	//描画処理
	void Render(float elapsedTime)override;
#ifdef USE_IMGUI
	//ImGui描画関数
	void ImGuiRender()override;
	void ImGuiOperationWindow();
	//Assetウィンドウ用のImGui描画関数
	void ImGuiTextWindow(float a_buttonWidth);
	void ImGuiAssetsWindow(float a_buttonWidth);
	void ImGuiAllCharactersWindow(float a_buttonWidth);
	//Slideウィンドウ用のImGui描画関数
	void ImGuiSlideWindow();
	void ImGuiCharactersWindow();
	void ImGuiEnterWindow();
	void ImGuiExcuteWindow();
	void ImGuiExitWindow();
	//Guizmo関数
	void ImGuizmoRender();
#endif // USE_IMGUI
	//画面サイズ変更時に呼ばれる関数
	void OnSizeChange()override;
private:
	//画面を４分割するための値
	struct ScreenSeparateLine
	{
		float rate[3] = { 0.3f,0.7f,0.7f };				//x成分は垂直方向の分割線のパラメーター(0.2fなら2:8で分割する)、y成分は水平方向の分割線のパラメーター(0.6fなら6:4で分割する)
		float beforeRate[3] = {};						//以前のrateを保存するための変数
		DirectX::XMFLOAT2 linePosition[3] = {};			//分割線のPosition
		DirectX::XMFLOAT2 lineSize[3] = {};				//分割線のSize
		bool lineTouchFlag[3] = { false,false,false };	//マウスで分割線を移動するための変数
		bool lineHoldFlag[3] = { false,false,false };	//				↓↓↓

		void MouseHitCheck();							//マウスが分割線と触れているかを確認する関数
		void LineMove();								//マウスで分割線を操作するための関数

		enum class SeparateType
		{
			Vertical,
			LeftHorizontal,
			RightHorizontal,
			Max,
		};
	};
	ScreenSeparateLine m_screenSeparateLine;

	//ScreenView(どのように描画されるか確認できる)
	DirectX::XMFLOAT2 m_reviewScreenPos = {}; //position(Center)
	DirectX::XMFLOAT2 m_reviewScreenSize = {}; //サイズ
	DirectX::XMFLOAT2 m_reviewScreenLeftTopPos = {}; //position(LeftTop)
	DirectX::XMFLOAT2 m_reviewScreenRightBottomPos = {}; //position(RightBottom)
	DirectX::XMFLOAT4 m_reviewScreenColor = { 1.0f,1.0f,1.0f,1.0f }; //色
	DirectX::XMFLOAT2 m_reviewScreenNormalSize = { 16.0f,9.0f }; //縦横比
	float m_reviewScreenAspectRate = 0.0f; //アスペクト比
	bool m_reviewFullScreenTestFlag = false; //フルスクで表示するかのフラグ

	//ImGuiウィンドウサイズ
	float m_charactersWindowWidth = 0.0f; //CharacterWindowの横幅
	float m_eventWindowRate = 0.0f; //EventWindow1つ辺りの横幅((0.0f ~ 1.0f) / 3)
	float m_eventWindowWidth = 0.0f; //EventWindow1つ辺りの横幅
	DirectX::XMFLOAT2 m_eventWindowDrawStartPos = {}; //eventWindow(最も左)の描画開始位置
	float m_slideWindowHeight = 0.0f; //SlideWindowの高さ
	float m_reviewOffsetScale = 0.85f; //reviewScreenをどのくらいの割合で描画するか
	DirectX::XMFLOAT2 m_blackSpaceSize{}; //reviewScreenを描画した後の黒い余白のサイズ

	//テキストウィンドウ用ポインタ変数
	std::vector<std::unique_ptr<SignBoard>> m_signBoards;

	//チャプター用ポインタ変数
	std::unique_ptr<Chapter> m_chapter;
	int m_slideIndex = 0; //何枚目のスライドを選択しているかの変数(0なら1枚目)
	Slide* m_currentSlide;

	//Guizmo用変数
	bool m_usingGuizmo = false;	//Guizmo使用中かのフラグ
	int m_guizmoType = 0; //Guizmoの操作タイプ(移動 or 拡大・縮小)

	std::vector<float> m_lines; //線の配列
	float lineNormalizeWallDistance = 0.1f; //0.0f ~ 1.0f
	float lineNormalizeDistance = 0.1f;  //0.0f ~ 1.0f

	std::unordered_map<std::string,std::unique_ptr<RectUI>> m_rectUIs; //当り判定が矩形のUI配列
	
	enum class SpriteKind
	{
		White,
		Number,
	};
};