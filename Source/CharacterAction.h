#pragma once
#include <DirectXMath.h>
#include <string>
#include "Json.h"
#include "Character.h"

class Character;

class CharacterAction
{
public:
	enum class Attribute
	{
		Enter,
		Execute,
		Exit,
	};

	struct Data
	{
		std::weak_ptr<Character> characterPtr;
		std::string name = {};
		int index = -1;
		float timer = 0.0f;
		float power = 0.0f;
		bool isEnd = false;
	};

	CharacterAction() = default;
	CharacterAction(Data a_data) :m_data(a_data) {}
	virtual ~CharacterAction() = default;

	//初期化関数
	virtual void Initialize() { m_data.isEnd = false; } //実行時処理
	virtual void Finalize() { m_data.isEnd = false; } //終了時処理
	//アクションを行うために呼ぶ関数(スライドに切り替わった時、実行時、スライド切り替え時)
	virtual void Enter() {};
	virtual bool Execute(float a_elapsedTime) { return true; }
	virtual void Exit() {};

	virtual void OperateData() {} //データ操作
	virtual void ShowEnter() {} //Enter用パラメーター操作
	virtual void ShowExecute() {} //Execute用パラメーター操作
	virtual void ShowExit() {} //Exit用パラメーター操作
	virtual void ToolUpdate(float a_elapsedTime) {} //パラメーター更新(Tool)


	//Guizmo用関数
	virtual DirectX::XMMATRIX GuizmoScale() { return DirectX::XMMatrixIdentity(); }
	virtual DirectX::XMMATRIX GuizmoRotate() { return DirectX::XMMatrixIdentity(); }
	virtual DirectX::XMMATRIX GuizmoTranslate() { return DirectX::XMMatrixIdentity(); }
	virtual void GuizmoDecompose(DirectX::XMMATRIX a_taransform) {} //パラメーター反映
	virtual void UiRender(DirectX::XMFLOAT2 a_screenLeftTopPos, DirectX::XMFLOAT2 a_screenSize) {}

	Data m_data;

	template<class T>
	void serialize(T& archive)
	{
		archive
		(
			CEREAL_NVP(m_data.characterPtr),
			CEREAL_NVP(m_data.name),
			CEREAL_NVP(m_data.index),
			CEREAL_NVP(m_data.power)
		);
	}

	bool useImGui = false;
	float angle = 0.0f;
};