#include "Timer.h"

Timer::Timer(TimerType c_type, float c_endTime, float startTime, bool c_controllFlag, bool c_loop)
	:type(static_cast<int>(c_type)), end_time(c_endTime), start_time(startTime), first_controll_flag(c_controllFlag), loop(c_loop)
{
	Initialize();
}

//�������֐�
void Timer::Initialize()
{
	main_timer = start_time;
	controll_flag = first_controll_flag;
}

//�K�莞�ԂɒB������true���A����ȊO�̏ꍇ��false��Ԃ�
bool Timer::Update(float elapsedTime)
{
	//���Z�����A���Z�����ŏ�����ς���
	switch (static_cast<TimerType>(type))
	{
	case TimerType::Additive:
		main_timer += elapsedTime;
		if (main_timer > end_time)//�K�莞�ԂɒB������
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
		if (main_timer < end_time)//�K�莞�ԂɒB������
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
	//���Z�����A���Z�����ŏ�����ς���
	switch (static_cast<TimerType>(type))
	{
	case TimerType::Additive:
		main_timer += elapsedTime;
		if (main_timer >= end_time)//�K�莞�ԂɒB������
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
		if (main_timer <= end_time)//�K�莞�ԂɒB������
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
