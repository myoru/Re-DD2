#pragma once
#include <DirectXMath.h>
#include <string>
#include "Json.h"

class Action
{
public:
	struct Data
	{
		std::string name = {};
		int index = -1;
		float timer = 0.0f;
		float power = 0.0f;
		bool isEnd = false;

		/*template<class T>
		void serialize(T& archive)
		{
			archive
			(
				CEREAL_NVP(name),
				CEREAL_NVP(timer),
				CEREAL_NVP(power),
				CEREAL_NVP(index),
				CEREAL_NVP(isEnd)
			);
		}*/
	};

	Action() = default;
	Action(Data a_data) :m_data(a_data) {}
	~Action() {}

	virtual void Excute(float a_elapsedTime) {};

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