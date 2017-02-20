#include "NormalizedRange.h"
#include "AuxFunc.h"

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
	float midpointNormalized = (mid - start) / (end - start);
	float skew = -midpointNormalized / (midpointNormalized - 1.f);
	return NormalizedRange(start, end, skew);
}

bool NormalizedRange::isInRange(float unnormalizedValue) const
{
	return unnormalizedValue >= start && unnormalizedValue <= end;
}

float NormalizedRange::fromNormalized(float normalizedValue) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (normalizedValue < 0.f || normalizedValue > 1.f)
		throw "not in range";
#endif
	normalizedValue = (normalizedValue * skew) / ((1.f - normalizedValue) + (normalizedValue * skew));
	return start + (end - start) * normalizedValue;
}

float NormalizedRange::toNormalized(float unnormalizedValue) const
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (unnormalizedValue < start || unnormalizedValue > end)
		throw "not in range";
#endif
	float normalizedValue = (unnormalizedValue - start) / (end - start);
	normalizedValue = normalizedValue / (normalizedValue + skew - skew * normalizedValue);
#ifndef _BLANKENHAIN_RUNTIME_MODE
	normalizedValue = aux::min(1.f, aux::max(0.f, normalizedValue));
#endif
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