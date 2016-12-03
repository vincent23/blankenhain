#include "Filter.h"

#include "Constants.h"

#include <cmath>

void Filter::recomputeCoefficients(double frequency, double Q)
{
	g = std::tan(constants::pi * frequency / constants::sampleRate);
	k = 1. / Q;
	a1 = 1. / (1. + g * (g + k));
	a2 = g * a1;
	a3 = g * a2;
}

Sample Filter::tickLow(const Sample& in)
{
	tick(in);
	return v2;
}

Sample Filter::tickHigh(const Sample& in)
{
	tick(in);
	return in - v1 * k - v2;
}

Sample Filter::tickBand(const Sample& in)
{
	tick(in);
	return v1;
}

Sample Filter::tickNotch(const Sample& in)
{
	tick(in);
	return in - v1 * k;
}

Sample Filter::tickPeak(const Sample& in)
{
	tick(in);
	return in - v1 * k - v2 * 2.;
}

Sample Filter::tickAll(const Sample& in)
{
	tick(in);
	return in - v1 * (2. * k);
}

void Filter::tick(const Sample& v0)
{
	v3 = v0 - ic2eq;
	v1 = ic1eq * a1 + v3 * a2;
	v2 = ic2eq + ic1eq * a2 + v3 * a3;
	ic1eq = v1 * 2.- ic1eq;
	ic2eq = v2 * 2. - ic2eq;
}