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

private:
	TextReadSpeed textReadSpeed = TextReadSpeed::Normal;



};