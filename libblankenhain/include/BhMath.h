#pragma once

namespace BhMath {
	float log2(float x);
	float log10(float x);
	float exp2(float x);
	float exp(float x);
	float fmod(float x, float y);
	float pow(float x, float y);
	float sqrt(float x);
	float tan(float x);
	float sin(float x);
	float abs(float x);

	//via http://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
	double fastPow(double a, double b);
}