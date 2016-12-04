#include "Filter.h"

#include "Constants.h"

#include <cmath>

void Filter::setLowPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(0);
	m1 = Sample(0);
	m2 = Sample(1);
}

void Filter::setHighPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(1);
	m1 = -k;
	m2 = Sample(-1);
}

void Filter::setBandPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(0);
	m1 = Sample(1);
	m2 = Sample(0);
}

void Filter::setNotch(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(1);
	m1 = -k;
	m2 = Sample(0);
}

void Filter::setBell(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q * A, 1);
	m0 = Sample(1);
	m1 = k * (A * A - 1);
	m2 = Sample(0);
}

void Filter::setLowShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q, 1. / std::sqrt(A));
	m0 = Sample(1);
	m1 = k * (A - 1);
	m2 = Sample(A * A - 1);
}

void Filter::setHighShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q, std::sqrt(A));
	m0 = Sample(A * A);
	m1 = k * ((1 - A) * A);
	m2 = Sample(1 - A * A);
}

void Filter::setPeak(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(1);
	m1 = Sample(-k);
	m2 = Sample(-2);
}

void Filter::setAll(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = Sample(1);
	m1 = Sample(-2) * k;
	m2 = Sample(0);
}

Sample Filter::tick(const Sample& v0)
{
	v3 = v0 - ic2eq;
	v1 = a1 * ic1eq + a2 * v3;
	v2 = ic2eq + a2 * ic1eq + a3 * v3;
	ic1eq = v1 * 2. - ic1eq;
	ic2eq = v2 * 2. - ic2eq;
	return m0 * v0 + m1 * v1 + m2 * v2;
}

void Filter::recomputeCoefficients(double frequency, double Q, double gFactor)
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