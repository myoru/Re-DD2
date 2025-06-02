#pragma once
#include <DirectXMath.h>

enum class TimerType
{
	Additive,
	Subtractive,
};

class Timer
{
public:
	Timer(TimerType c_type, float c_endTime, float startTime = 0.0f, bool c_controllFlag = false, bool c_loop = false);
	~Timer() {};

	void Initialize();
	bool Update(float elapsedTime);

	float GetMainTimer() { return main_timer; }
	float GetEndTime() { return end_time; }
	float GetStartTime() { return start_time; }
	bool GetControllFlag() { return controll_flag; }
	void SetControllFlag(bool set) { controll_flag = set; }
	bool GetFirstControllFlag() { return first_controll_flag; }

private:
	int type;
	float main_timer;
	float end_time;
	float start_time;
	bool loop;
	bool controll_flag;
	bool first_controll_flag;
};



class SimpleTimer
{
public:
	SimpleTimer(TimerType c_type, float c_endTime, float startTime = 0.0f, bool c_loop = false);
	~SimpleTimer() {};

	void Initialize();
	void Initialize(float newEndTime, float newStartTime);
	bool Update(float elapsedTime);

	float GetMainTimer() { return main_timer; }
	float GetEndTime() { return end_time; }
	float GetStartTime() { return start_time; }

private:
	int type;
	float main_timer;
	float end_time;
	float start_time;
	bool loop;
};