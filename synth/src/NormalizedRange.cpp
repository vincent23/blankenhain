#include "NormalizedRange.h"
#include <cmath>

NormalizedRange::NormalizedRange(float start_, float end_, float skew_)
	: start(start_), end(end_), skew(skew_)
{
}

float NormalizedRange::fromNormalized(float normalizedValue) const
{
	if (skew != 1.f && normalizedValue > 0.f) {
		normalizedValue = std::exp(std::log(normalizedValue) / skew);
	}
	return start + (end - start) * normalizedValue;
}

float NormalizedRange::toNormalized(float unnormalizedValue) const
{
	float normalizedValue = (unnormalizedValue - start) / (end - start);
	if (skew != 1) {
		normalizedValue = std::pow(normalizedValue, skew);
	}
	return normalizedValue;
}
