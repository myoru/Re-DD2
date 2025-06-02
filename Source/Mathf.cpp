#include "Mathf.h"
#include <stdlib.h>

//���`���
float Mathf::Lerp(float a, float b, float t)
{
	return a * (1.0f - t) + (b * t);
}

float Mathf::RandomRange(float min, float max)
{
	//0.0 ~ 1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));

	//min ~ max �܂ł̃����_���l�ɕϊ�
	return min + (max - min) * value;

}
