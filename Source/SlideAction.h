#pragma once
#include <DirectXMath.h>
#include <string>
#include "Json.h"

class SlideAction
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
		std::string name = {};
		int index = -1;
		int mode = 0;
		float timer = 0.0f;
		float power = 0.0f;
		bool isEnd = false;
	};

	SlideAction() = default;
	SlideAction(Data a_data) :m_data(a_data) {}
	virtual ~SlideAction() = default;

	virtual void Enter() {};
	virtual bool Execute(float a_elapsedTime) { return true; };
	virtual void Exit() {};

	virtual void OperateData() {}
	virtual void ShowEnter() {}
	virtual void ShowExecute() {}
	virtual void ShowExit() {}

	Data m_data;

	template<class T>
	void serialize(T& archive)
	{
		archive
		(
			CEREAL_NVP(m_data.name),
			CEREAL_NVP(m_data.power),
			CEREAL_NVP(m_data.index)
		);
	}
};