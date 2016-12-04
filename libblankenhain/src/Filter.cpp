#include "Filter.h"

#include "Constants.h"

#include <cmath>

void Filter::recomputeCoefficients(double frequency, double Q)
{
	recomputeCoefficientsInternal(frequency, Q, 1.);
}

void Filter::recomputeCoefficientsBell(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficientsInternal(frequency, Q * A, 1.);
	m1 = k * (A * A - 1.);
}

void Filter::recomputeCoefficientsLowShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficientsInternal(frequency, Q, 1. / std::sqrt(A));
	m1 = k * (A - 1);
	m2 = Sample(A * A - 1);
}

void Filter::recomputeCoefficientsHighShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficientsInternal(frequency, Q, std::sqrt(A));
	m0 = Sample(A * A);
	m1 = Sample(k * (1 - A) * A);
	m2 = Sample(1 - A * A);
}

Sample Filter::tickLow(const Sample& in)
{
	tick(in);
	return v2;
}

Sample Filter::tickHigh(const Sample& in)
{
	tick(in);
	return in - k * v1 - v2;
}

Sample Filter::tickBand(const Sample& in)
{
	tick(in);
	return v1;
}

Sample Filter::tickNotch(const Sample& in)
{
	tick(in);
	return in - k * v1;
}

Sample Filter::tickBell(const Sample& in)
{
	tick(in);
	return in + m1 * v1;
}

Sample Filter::tickLowShelf(const Sample& in)
{
	tick(in);
	return in + m1 * v1 + m2 * v2;
}

Sample Filter::tickHighShelf(const Sample& in)
{
	tick(in);
	return m0 * in + m1 * v1 + m2 * v2;
}

Sample Filter::tickPeak(const Sample& in)
{
	tick(in);
	return in - k * v1 - v2 * 2.;
}

Sample Filter::tickAll(const Sample& in)
{
	tick(in);
	return in - k * v1 * 2.;
}

void Filter::tick(const Sample& v0)
{
	v3 = v0 - ic2eq;
	v1 = a1 * ic1eq + a2 * v3;
	v2 = ic2eq + a2 * ic1eq + a3 * v3;
	ic1eq = v1 * 2. - ic1eq;
	ic2eq = v2 * 2. - ic2eq;
}

void Filter::recomputeCoefficientsInternal(double frequency, double Q, double gFactor)
{
	double g_ = std::tan(constants::pi * frequency / constants::sampleRate) * gFactor;
	double k_ = 1. / Q;
	double a1_ = 1. / (1. + g_ * (g_ + k_));
	double a2_ = g_ * a1_;
	double a3_ = g_ * a2_;

	g = Sample(g_);
	k = Sample(k_);
	a1 = Sample(a1_);
	a2 = Sample(a2_);
	a3 = Sample(a3_);
}