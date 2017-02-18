#include "Sample.h"

#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>
#include "AlignedType.h"

Sample::Sample() : v(_mm_set1_pd(0.))
{ }

Sample::Sample(double singleSample)
{
	v = _mm_set1_pd(singleSample);
}

Sample::Sample(__m128d in)
{
	v = in;
}

Sample::Sample(const Sample& x)
{
	v = x.v;
}

Sample::Sample(double sampleLeft, double sampleRight)
{
	v = _mm_set_pd(sampleRight, sampleLeft);
}

Sample _vectorcall Sample::load_aligned(const double* ptr)
{
	return _mm_load_pd(ptr);
}

void _vectorcall Sample::store_aligned(double* ptr) const
{
	_mm_store_pd(ptr, v);
}

Sample& _vectorcall Sample::operator=(const Sample& other)
{
	v = other.v;
	return *this;
}

Sample& _vectorcall Sample::operator+=(const Sample& other)
{
	v = _mm_add_pd(v, other.v);
	return *this;
}

Sample& _vectorcall Sample::operator-=(const Sample& other)
{
	v = _mm_sub_pd(v, other.v);
	return *this;
}

Sample& _vectorcall Sample::operator*=(const Sample& other)
{
	v = _mm_mul_pd(v, other.v);
	return *this;
}

Sample& _vectorcall Sample::operator/=(const Sample& other)
{
	v = _mm_div_pd(v, other.v);
	return *this;
}

Sample _vectorcall Sample::operator+(const Sample& other) const
{
	Sample result(v);
	result += other;
	return result;
}

Sample _vectorcall Sample::operator-(const Sample& other) const
{
	Sample result(v);
	result -= other;
	return result;
}

Sample _vectorcall Sample::operator*(const Sample& other) const
{
	Sample result(v);
	result *= other;
	return result;
}

Sample _vectorcall Sample::operator/(const Sample& other) const
{
	Sample result(v);
	result /= other;
	return result;
}

Sample _vectorcall Sample::operator-() const
{
	// flip sign bit
	return _mm_xor_pd(v, Sample(-0.f).v);
}

Sample _vectorcall Sample::abs() const
{
	// delete sign bits
	return _mm_andnot_pd(Sample(-0.f).v, v);
}

Sample _vectorcall Sample::sign() const
{
	// extract sign bit, then combine it with 1
	__m128d signBits = _mm_and_pd(v, Sample(-0.f).v);
	return _mm_or_pd(signBits, Sample(1).v);
}

Sample _vectorcall Sample::sqrt() const
{
	Sample radicand = _mm_max_pd(Sample(0).v, v);
	return _mm_sqrt_pd(radicand.v);
}

double _vectorcall Sample::avgValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return (lr[0] + lr[1]) / 2.;
}

double _vectorcall Sample::maxValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[1] : lr[0];
}

double _vectorcall Sample::minValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[0] : lr[1];
}

void Sample::replaceLeftChannel(Sample const& in )
{
	this->v = _mm_shuffle_pd(this->v, in.v, _MM_SHUFFLE2(1, 0));
}
void Sample::replaceRightChannel(Sample const& in)
{
	this->v = _mm_shuffle_pd(this->v, in.v, _MM_SHUFFLE2(0, 1));
}