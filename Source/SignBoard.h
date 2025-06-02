#pragma once
#include <d3d11.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "TextRenderer.h"
#include "Sprite.h"
#include "Json.h"

using namespace DirectX;

class SignBoard
{
public:
	SignBoard(ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext, std::string a_jsonFileName);
	~SignBoard();

	void Update(float a_elapsedTime, DirectX::XMFLOAT2 a_screenLeftTopPosition = { 0.0f,0.0f }, DirectX::XMFLOAT2 a_screenSize = { -1.0f,-1.0f });
	void BoardRender();
	void TextRender(FXMVECTOR a_textColor = Colors::Red);
	void ToolRender(bool a_fullscreen,FXMVECTOR a_textColor = Colors::White);

	Sprite* GetBoardSprite() { return m_boardSpr.get(); }
	
#ifdef USE_IMGUI
	
#endif
	//Jsonを用いて入出力するデータ、または必要なデータ
	struct JsonElements
	{
		std::string boardFilePath{};							//ボードに使う画像のパス(相対)
		std::string fontFilePath{};								//テキストに使うフォントのパス(相対)
		char jsonFileName[256]{};								//出力するJsonファイルの名前

		std::string text{};										//描画する文字列
		DirectX::XMFLOAT2 boardPosition{};						//スクリーン空間
		DirectX::XMFLOAT2 boardLocalPosition{ 0.5f,0.5f };		//スクリーンサイズに対して何割の位置であるか(X成分が0.7fなら横軸の位置は画面の横幅を7:3で分割する位置)
		DirectX::XMFLOAT2 boardSize{};							//スクリーン空間
		DirectX::XMFLOAT2 boardLocalSize{0.9f,0.0f};			//スクリーンサイズに対して何割の大きさであるか(X成分が0.7fなら横幅は画面の横幅の大きさに0.7fかけた大きさ)

		int textAlignment = static_cast<int>(TextRenderer::TextAlignment::Center);	//アライメント(左端揃え、中央揃え、右端揃え)
		DirectX::XMFLOAT2 textDrawableAreaPosition{};								//スクリーン空間
		DirectX::XMFLOAT2 textDrawableAreaLocalPosition{ 0.5f,0.5f };				//ボードサイズに対して何割の位置であるか(X成分が0.7fなら横軸の位置はボードの横幅を7:3で分割する位置)
		DirectX::XMFLOAT2 textDrawableAreaSize{};									//スクリーン空間
		DirectX::XMFLOAT2 textDrawableAreaLocalSize{0.9f,0.9f};						//ボードサイズに対して何割の大きさであるか(X成分が0.7fなら横幅はボードの横幅の大きさに0.7fかけた大きさ)

		DirectX::XMFLOAT2 textDrawStartPosition = {};						//スクリーン空間
		DirectX::XMFLOAT2 textDrawStartLocalPosition = { 0.5f,0.5f };		//テキスト描画可能エリアのサイズに対して何割の位置であるか(X成分が0.7fなら横軸の位置はテキスト描画可能エリアの横幅を7:3で分割する位置)
		float textScale{};													//テキスト描画時に使う
		float textLocalHeight{0.3f};										//テキスト描画可能エリアのサイズに対して何割の大きさであるか(0.7fならテキスト描画可能エリアの縦幅の大きさに0.7fかけた大きさ)
		float localLineSpace = 0.0f;										//行間隔(ローカル)
		float lineSpace = 0.0f;												//行間隔
		bool autoAspectChange = true;										//ボードのアスペクト比を固定するかのフラグ

		template<class T>
		void serialize(T& archive)
		{
			archive(
				CEREAL_NVP(boardFilePath),
				CEREAL_NVP(fontFilePath),
				CEREAL_NVP(boardLocalPosition),
				CEREAL_NVP(boardLocalSize),
				CEREAL_NVP(textAlignment),
				CEREAL_NVP(textDrawableAreaLocalPosition),
				CEREAL_NVP(textDrawableAreaLocalSize),
				CEREAL_NVP(textDrawStartLocalPosition),
				CEREAL_NVP(textLocalHeight),
				CEREAL_NVP(localLineSpace),
				CEREAL_NVP(autoAspectChange));
		}
	};
	JsonElements jElements;
private:
	std::unique_ptr<Sprite> m_whiteSpr;
	std::unique_ptr<Sprite> m_boardSpr;
	std::unique_ptr<TextRenderer> m_textRenderer;
public:
	char m_inputBuffer[256] = {};
};
