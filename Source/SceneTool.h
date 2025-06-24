#pragma once
#include "Scene.h"
#include "SignBoard.h"
#include "Chapter.h"
#include "RectUI.h"
#include <unordered_map>


class SceneTool : public Scene
{
public:
	SceneTool(){};
	~SceneTool() override {}

	//初期化
	void Initialize() override;
	//終了化
	void Finalize()override;
	//更新処理
	void Update(float elapsedTime)override;
	void ModeChange(); //モード切り替え関数
	void ReviewBoardUpdate();
	void CharactersUpdate(float a_elapsedTime); //キャラクター更新処理
	void RectUIUpdate(float a_elapsedTime); //矩形UI更新処理
	void RectUIHitCheck(); //矩形UI当り判定
	void SlideJumpUpdate();
	void SlideJumpHitCheck(float a_elapsedTime);
	void LineUpdate();
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

	//ScreenView(どのように描画されるか確認できるエリア)
	DirectX::XMFLOAT2 m_reviewScreenPos = {}; //position(Center)
	DirectX::XMFLOAT2 m_reviewScreenLeftTopPos = {}; //position(LeftTop)
	DirectX::XMFLOAT2 m_reviewScreenRightBottomPos = {}; //position(RightBottom)
	DirectX::XMFLOAT2 m_reviewScreenSize = {}; //サイズ
	DirectX::XMFLOAT4 m_reviewScreenColor = { 1.0f,1.0f,1.0f,1.0f }; //色
	DirectX::XMFLOAT2 m_reviewScreenNormalSize = { 16.0f,9.0f }; //縦横比
	float m_reviewScreenAspectRate = 0.0f; //アスペクト比
	//bool m_reviewFullScreenTestFlag = false; //フルスクで表示するかのフラグ

	//ImGuiウィンドウ関連の変数
	float m_charactersWindowWidth = 0.0f; //CharacterWindowの横幅
	float m_eventWindowRate = 0.0f; //EventWindow1つ辺りの横幅((0.0f ~ 1.0f) / 3)
	float m_eventWindowWidth = 0.0f; //EventWindow1つ辺りの横幅
	DirectX::XMFLOAT2 m_eventWindowDrawStartPos = {}; //eventWindow(最も左)の描画開始位置
	float m_slideWindowHeight = 0.0f; //SlideWindowの高さ
	float m_reviewOffsetScale = 0.85f; //reviewScreenをどのくらいの割合で描画するか
	DirectX::XMFLOAT2 m_blackSpaceSize{}; //reviewScreenを描画するエリアのサイズ
	DirectX::XMFLOAT2 m_remainingBlackSpaceSize{}; //reviewScreenを描画した後の黒い余白のサイズ

	//テキストウィンドウ用ポインタ変数
	std::vector<std::unique_ptr<SignBoard>> m_signBoards;

	//チャプター用ポインタ変数
	std::unique_ptr<Chapter> m_chapter; //Chapterポインタ

	//スライド用変数
	DirectX::XMFLOAT2 slideJumpUIDrawStartPos = {}; //スライド移動用UIの描画開始位置
	DirectX::XMFLOAT2 m_slideJumpUISize = {}; //スライド移動用のスペースの描画サイズ
	float m_slideJumpUIHeightRate = 0.8f; //黒い余白に対して、スライド移動用のスペースをどれくらいの大きさ(height)で描画するかの値
	float m_slideJumpUIDrawWidth = 0.0f; //スライド移動用UIの一つ分の幅(当り判定はこの幅にslideJumpUIScaleRateをかけた物で行う)
	int m_touchSlideIndex = -1; //何番目のスライドジャンプ用UIに触れているか
	float slideJumpUIScaleRate = 0.8f; //ジャンプ用UI同士が間隔をあけて並ぶようにするために用意した変数
	float m_slideClickTimer = 0.0f; //ダブルクリックを検知するためのタイマー()
	float m_doubleClickTime = 0.5f; //ダブルクリック入力を許容する時間

	//Guizmo用変数
	bool m_usingGuizmo = false;	//Guizmo使用中かのフラグ
	int m_guizmoType = 0; //Guizmoの操作タイプ(移動 or 拡大・縮小)
	int m_testNum = 0;

	//グリッド線用の変数
	std::vector<float> m_lines; //線の配列
	float lineNormalizeWallDistance = 0.1f; //0.0f ~ 1.0f
	float lineNormalizeDistance = 0.1f;  //0.0f ~ 1.0f

	std::unordered_map<std::string,std::unique_ptr<RectUI>> m_rectUIs; //当り判定が矩形のUI配列
	
	enum class SpriteKind
	{
		White,
		Number,
	};

	enum class Mode
	{
		Edit, //編集
		Check, //確認
		Slideshow, //スライドショー
	};
	Mode m_mode = Mode::Edit; //現在のモード
};