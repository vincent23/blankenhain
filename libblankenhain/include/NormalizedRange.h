#pragma once

class NormalizedRange
{
public:
	NormalizedRange(float start = 0.f, float end = 1.f, float skew = 1.f);
	NormalizedRange(bool itReallyDoesNotMatterIfThisIsTrueOrFalse);

	/**
	* Constructs a @a NormalizedRange with a skew factor so that the normalized value 0.5 is equal to @a mid.
	* @param start Value that will be mapped to 0
	* @param mid Value that will be mapped to 0.5
	* @param end Value that will be mapped to 1
	*/
	static NormalizedRange fromMidpoint(float start, float mid, float end);

	float fromNormalized(float normalizedValue) const;
	float toNormalized(float unnormalizedValue) const;

	float getStart() const;
	float getEnd() const;
	float getSkew() const;

private:
	float start, end, skew;
};

