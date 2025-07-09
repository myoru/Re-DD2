#pragma once
#include "Singleton.h"

class Options :public Singleton<Options>
{
public:
	enum class TextReadSpeed
	{
		MostSlow,
		Slow,
		Normal,
		Fast,
		MostFast,
		Max
	};

	Options() :Singleton() {}
	~Options()override {}

	bool InitializeElements()override
	{
		return true;
	}

	TextReadSpeed GetTextReadSpeed() { return textReadSpeed; }
	void SetTextReadSpeed(int a_textReadSpeed) { textReadSpeed = static_cast<TextReadSpeed>(a_textReadSpeed); }
	float GetTextReadSpeedValue() 
	{
		int _value = 0;
		switch (textReadSpeed)
		{
		case TextReadSpeed::MostSlow:
			_value = 10.0f; break;
		case Options::TextReadSpeed::Slow:
			_value = 15.0f; break;
		case Options::TextReadSpeed::Normal:
			_value = 23.0f; break;
		case Options::TextReadSpeed::Fast:
			_value = 32.0f; break;
		case Options::TextReadSpeed::MostFast:
			_value = 40.0f; break;
		}

		return _value;
	}


private:
	TextReadSpeed textReadSpeed = TextReadSpeed::Normal;



};