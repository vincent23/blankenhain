#ifndef NORMALIZEDRANGE_H_INCLUDED
#define NORMALIZEDRANGE_H_INCLUDED

class NormalizedRange
{
public:
	NormalizedRange(float start = 0.f, float end = 1.f, float skew = 1.f);

	float fromNormalized(float normalizedValue) const;
	float toNormalized(float unnormalizedValue) const;

private:
	const float start, end, skew;
};
#endif

