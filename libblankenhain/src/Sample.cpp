#include "Sample.h"

#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>

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

Sample::Sample(double sampleLeft, double sampleRight)
{
	v = _mm_set_pd(sampleRight, sampleLeft);
}

Sample _vectorcall Sample::load_aligned(const double* ptr)
{
	return _mm_load_pd(ptr);
}

void* Sample::operator new[](unsigned int size)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (size == 0) {
		throw("Alloc of size == 0\n");
		return;
	}
#endif
	return _aligned_malloc(size, 16u);
}

void Sample::operator delete[](void *p) throw()
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (p == nullptr)
	{
		throw ("Tried to delete nullptr\n");
		return;
	}
#endif
	_aligned_free(p);
}

void Sample::store_aligned(double* ptr) const
{
	_mm_store_pd(ptr, v);
}

Sample _vectorcall Sample::operator+=(Sample other)
{
	v = _mm_add_pd(v, other.v);
	return *this;
}

Sample _vectorcall Sample::operator-=(Sample other)
{
	v = _mm_sub_pd(v, other.v);
	return *this;
}

Sample _vectorcall Sample::operator*=(Sample other)
{
	v = _mm_mul_pd(v, other.v);
	return *this;
}

Sample _vectorcall Sample::operator/=(Sample other)
{
	v = _mm_div_pd(v, other.v);
	return *this;
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

double Sample::avgValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return (lr[0] + lr[1]) / 2.;
}

double Sample::maxValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[1] : lr[0];
}

double Sample::minValue() const
{
	alignas(16) double lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[0] : lr[1];
}

void _vectorcall Sample::replaceLeftChannel(Sample in)
{
	v = _mm_shuffle_pd(in.v, v, _MM_SHUFFLE2(1, 0));
}

void _vectorcall Sample::replaceRightChannel(Sample in)
{
	v = _mm_shuffle_pd(v, in.v, _MM_SHUFFLE2(1, 0));
}

Sample __vectorcall Sample::flippedChannels() const
{
	return _mm_shuffle_pd(v, v, _MM_SHUFFLE2(0, 1));
}