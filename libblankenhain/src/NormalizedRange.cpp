#include "NormalizedRange.h"
#include <cmath>

/**
* Maps numbers from an arbitrary range to [0.f, 1.f]
* By giving a skew parameter, the range can be adjusted:
* Skew = 1.f : Linear Mapping:
* Skew > 1.f : Values at END of unnormalized range can be adjusted more precisely
* 0.f < Skew < 1.f: Values at BEGINNING of unnormalized range can be adjusted more precisely
*
* You are also able to let the skew-parameter be determined by a desired "midpoint" through
* the fromMidpoint() function.
*/
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
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (normalizedValue < 0.f || normalizedValue > 1.f)
		throw ("normalized value out of range\n");
#endif
	if (skew != 1.f && normalizedValue > 0.f) {
		normalizedValue = std::pow(normalizedValue, 1.f / skew);
	}
	return start + (end - start) * normalizedValue;
}

float NormalizedRange::toNormalized(float unnormalizedValue) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (unnormalizedValue < start || unnormalizedValue > end) 
		throw ("Blankenhain Range Assert: Unnormalized not in Range");
#endif

	float normalizedValue = (unnormalizedValue - start) / (end - start);
	if (skew != 1) {
		normalizedValue = std::pow(normalizedValue, skew);
	}
	return normalizedValue;
}

float NormalizedRange::getStart() const
{
	return start;
}

float NormalizedRange::getEnd() const
{
	return end;
}

float NormalizedRange::getSkew() const
{
	return skew;
}