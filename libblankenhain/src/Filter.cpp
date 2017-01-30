#include "Filter.h"

#include "Constants.h"

#include <cmath>

template Filter<Sample>;
template Filter<float>;

template <typename T>
void Filter<T>::setLowPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(0);
	m1 = T(0);
	m2 = T(1);
}

template <typename T>
void Filter<T>::setHighPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(1);
	m1 = T(-k);
	m2 = T(-1);
}

template <typename T>
void Filter<T>::setBandPass(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(0);
	m1 = T(1);
	m2 = T(0);
}

template <typename T>
void Filter<T>::setNotch(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(1);
	m1 = T(-k);
	m2 = T(0);
}

template <typename T>
void Filter<T>::setBell(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q * A, 1);
	m0 = T(1);
	m1 = T(k * T(A * A - 1));
	m2 = T(0);
}

template <typename T>
void Filter<T>::setLowShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q, 1. / std::sqrt(A));
	m0 = T(1);
	m1 = k * T(A - 1);
	m2 = T(A * A - 1);
}

template <typename T>
void Filter<T>::setHighShelf(double frequency, double Q, double gainInDb)
{
	double A = std::pow(10, gainInDb / 40.);
	recomputeCoefficients(frequency, Q, std::sqrt(A));
	m0 = T(A * A);
	m1 = k * T((1 - A) * A);
	m2 = T(1 - A * A);
}

template <typename T>
void Filter<T>::setPeak(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(1);
	m1 = T(-k);
	m2 = T(-2);
}

template <typename T>
void Filter<T>::setAll(double frequency, double Q)
{
	recomputeCoefficients(frequency, Q, 1);
	m0 = T(1);
	m1 = T(-2) * k;
	m2 = T(0);
}

template <typename T>
T Filter<T>::tick(const T& v0)
{
	v3 = v0 - ic2eq;
	v1 = a1 * ic1eq + a2 * v3;
	v2 = ic2eq + a2 * ic1eq + a3 * v3;
	ic1eq = v1 * T(2.) - ic1eq;
	ic2eq = v2 * T(2.) - ic2eq;
	return m0 * v0 + m1 * v1 + m2 * v2;
}

template <typename T>
void Filter<T>::recomputeCoefficients(double frequency, double Q, double gFactor)
{
	double g_ = std::tan(constants::pi * frequency / constants::sampleRate) * gFactor;
	double k_ = 1. / Q;
	double a1_ = 1. / (1. + g_ * (g_ + k_));
	double a2_ = g_ * a1_;
	double a3_ = g_ * a2_;

	g = T(g_);
	k = T(k_);
	a1 = T(a1_);
	a2 = T(a2_);
	a3 = T(a3_);
}