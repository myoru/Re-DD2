#include "Timer.h"

Timer::Timer(TimerType c_type, float c_endTime, float startTime, bool c_controllFlag, bool c_loop)
	:type(static_cast<int>(c_type)), end_time(c_endTime), start_time(startTime), first_controll_flag(c_controllFlag), loop(c_loop)
{
	Initialize();
}

//初期化関数
void Timer::Initialize()
{
	main_timer = start_time;
	controll_flag = first_controll_flag;
}

//規定時間に達したらtrueを、それ以外の場合はfalseを返す
bool Timer::Update(float elapsedTime)
{
	//加算方式、減算方式で処理を変える
	switch (static_cast<TimerType>(type))
	{
	case TimerType::Additive:
		main_timer += elapsedTime;
		if (main_timer > end_time)//規定時間に達したら
		{
			if (loop)
			{
				Initialize();
			}
			return true;
		}
		break;
	case TimerType::Subtractive:
		main_timer -= elapsedTime;
		if (main_timer < end_time)//規定時間に達したら
		{
			if (loop)
			{
				Initialize();
			}
			return true;
		}
		break;
	}

	return false;;
}



SimpleTimer::SimpleTimer(TimerType c_type,float c_endTime, float startTime, bool c_loop)
	:type(static_cast<int>(c_type)), end_time(c_endTime), start_time(startTime),loop(c_loop)
{
	Initialize();
}

void SimpleTimer::Initialize()
{
	main_timer = start_time;
}

void SimpleTimer::Initialize(float newEndTime,float newStartTime)
{
	end_time = newEndTime;
	start_time = newStartTime;
	main_timer = start_time;
}

bool SimpleTimer::Update(float elapsedTime)
{
	//加算方式、減算方式で処理を変える
	switch (static_cast<TimerType>(type))
	{
	case TimerType::Additive:
		main_timer += elapsedTime;
		if (main_timer >= end_time)//規定時間に達したら
		{
			if (loop)
			{
				Initialize();
			}
			return true;
		}
		break;
	case TimerType::Subtractive:
		main_timer -= elapsedTime;
		if (main_timer <= end_time)//規定時間に達したら
		{
			if (loop)
			{
				Initialize();
			}
			return true;
		}
		break;
	}

	return false;;
}
