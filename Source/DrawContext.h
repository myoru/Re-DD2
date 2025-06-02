#pragma once
#include <DirectXMath.h>

struct LuminanceConstants
{
	float m_smoothMin = 0.6f;
	float m_smoothMax = 0.8f;
	float m_gaussianSigma = 1.0f;
	float m_bloomIntensity = 1.0f;
	float m_exposure = 0.8f;
};