#pragma once

/**
 * Maps numbers from an arbitrary range to [0.f, 1.f]
 * By giving a skew parameter, the range can be adjusted:
 * Skew = 1.f : Linear Mapping:
 * Skew > 1.f : Values at END of unnormalized range can be adjusted more precisely
 * 0.f < Skew < 1.f: Values at BEGINNING of unnormalized range can be adjusted more precisely
 */
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

