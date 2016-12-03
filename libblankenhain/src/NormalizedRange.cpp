#include "NormalizedRange.h"
#include <cmath>

NormalizedRange::NormalizedRange(float start_, float end_, float skew_)
	: start(start_), end(end_), skew(skew_)
{
}

NormalizedRange::NormalizedRange(bool itReallyDoesNotMatterIfThisIsTrueOrFalse)
	: start(0.), end(1.), skew(1.)
{
}

NormalizedRange NormalizedRange::fromMidpoint(float start, float mid, float end)
{
	float skew = 1.f / std::log2((end - start) / (mid - start));
	return NormalizedRange(start, end, skew);
}


float NormalizedRange::fromNormalized(float normalizedValue) const
{
	if (skew != 1.f && normalizedValue > 0.f) {
		normalizedValue = std::pow(normalizedValue, 1.f / skew);
	}
	return start + (end - start) * normalizedValue;
}

float NormalizedRange::toNormalized(float unnormalizedValue) const
{
#ifdef BLANKENHAIN_CHECKS
	if (unnormalizedValue < start || unnormalizedValue > end) throw "Blankenhain Range Assert: Unnormalized not in Range";
#endif

	float normalizedValue = (unnormalizedValue - start) / (end - start);
	if (skew != 1) {
		normalizedValue = std::pow(normalizedValue, skew);
	}
	return normalizedValue;
}
