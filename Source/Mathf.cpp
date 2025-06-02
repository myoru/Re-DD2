#include "Mathf.h"
#include <stdlib.h>

//線形補間
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{
	//0.0 ~ 1.0の間までのランダム値
	float value = static_cast<float>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));

	//min ~ max までのランダム値に変換
	return min + (max - min) * value;

}
