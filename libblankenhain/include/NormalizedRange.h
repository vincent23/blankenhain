#pragma once

class NormalizedRange
{
public:
	NormalizedRange(float start = 0.f, float end = 1.f, float skew = 1.f);
  NormalizedRange(bool itReallyDoesNotMatterIfThisIsTrueOrFalse);

	float fromNormalized(float normalizedValue) const;
	float toNormalized(float unnormalizedValue) const;

private:
	float start, end, skew;
};

