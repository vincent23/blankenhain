#include "NormalizedRange.h"
#include "BhMath.h"

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
	float skew = 1.f / BhMath::log2((end - start) / (mid - start));
	return NormalizedRange(start, end, skew);
}

bool NormalizedRange::isInRange(float unnormalizedValue) const
{
	return unnormalizedValue >= start && unnormalizedValue <= end;
}

float NormalizedRange::fromNormalized(float normalizedValue) const
{
#ifndef _BLANKENHAIN_RUNTIME_MODE
	// Sometimes the ranges subtly over- or undershoot,
	// for convenience we clamp this in the VST - PluginIns
	if (normalizedValue < 0.f)
		normalizedValue = 0.f;
	else if (normalizedValue > 1.f)
		normalizedValue = 1.f;
#endif
	if (skew != 1.f && normalizedValue > 0.f) {
		normalizedValue = BhMath::pow(normalizedValue, 1.f / skew);
	}
	return start + (end - start) * normalizedValue;
}

float NormalizedRange::toNormalized(float unnormalizedValue) const
{
#ifndef _BLANKENHAIN_RUNTIME_MODE
	// Sometimes the ranges subtly over- or undershoot,
	// for convenience we clamp this in the VST - PluginIns
	if (unnormalizedValue < start)
		unnormalizedValue = start;
	else if (unnormalizedValue > end)
		unnormalizedValue = end;
#endif

	float normalizedValue = (unnormalizedValue - start) / (end - start);
	if (skew != 1) {
		normalizedValue = BhMath::pow(normalizedValue, skew);
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