#include "Sample.h"

#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

Sample::Sample()
	: v(
#ifndef _LIBBLANKENHAIN_NO_SSE
		_mm_set1_pd(0.)
#endif
	)
{
#ifdef _LIBBLANKENHAIN_NO_SSE
	v[0] = 0.;
	v[1] = 0.;
#endif
}

Sample::Sample(floatType singleSample)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_set1_pd(singleSample);
#else
	v[0] = singleSample;
	v[1] = singleSample;
#endif
}

#ifndef _LIBBLANKENHAIN_NO_SSE
Sample::Sample(__m128d in)
{
	v = in;
}
#endif

Sample::Sample(floatType sampleLeft, floatType sampleRight)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_set_pd(sampleRight, sampleLeft);
#else
	v[0] = sampleLeft;
	v[1] = sampleRight;
#endif
}

#ifndef _LIBBLANKENHAIN_NO_SSE
Sample _vectorcall Sample::load_aligned(const floatType* ptr)
{
	return _mm_load_pd(ptr);
}
#endif

extern "C" void* __cdecl aligned_malloc(
	size_t const size,
	size_t align
);

extern "C" void __cdecl aligned_free(void* const block);

void* Sample::operator new[](unsigned int size)
{
#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (size == 0) {
		throw("Alloc of size == 0\n");
		return;
	}
#endif
	return aligned_malloc(size, 16u);
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
	aligned_free(p);
}

// Todo Remove all calls to this
void Sample::store_aligned(floatType* ptr) const
{
#ifdef _LIBBLANKENHAIN_NO_SSE
	ptr[0] = v[0];
	ptr[1] = v[1];
#else
	_mm_store_pd(ptr, v);
#endif
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::operator+=(Sample other)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_add_pd(v, other.v);
#else
	v[0] += other.v[0];
	v[1] += other.v[1];
#endif
	return *this;
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::operator-=(Sample other)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_sub_pd(v, other.v);
#else
	v[0] -= other.v[0];
	v[1] -= other.v[1];
#endif
	return *this;
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::operator*=(Sample other)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_mul_pd(v, other.v);
#else
	v[0] *= other.v[0];
	v[1] *= other.v[1];
#endif
	return *this;
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::operator/=(Sample other)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_div_pd(v, other.v);
#else
	v[0] /= other.v[0];
	v[1] /= other.v[1];
#endif
	return *this;
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::abs() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	// delete sign bits
	return _mm_andnot_pd(Sample(-0.f).v, v);
#else
	Sample out = *this;
	if (v[0] < 0.)
		out.v[0] *= -1.;
	if (v[1] < 0.)
		out.v[1] *= -1.;
	return out;
#endif
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::sign() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	// extract sign bit, then combine it with 1
	__m128d signBits = _mm_and_pd(v, Sample(-0.f).v);
	return _mm_or_pd(signBits, Sample(1).v);
#else
	Sample out;
	if (v[0] < 0.)
		out.v[0] = -1.;
	else
		out.v[0] = 1.;
	if (v[1] < 0.)
		out.v[1] = -1.;
	else
		out.v[1] = 1.;
	return out;
#endif
}

Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::sqrt() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	Sample radicand = _mm_max_pd(Sample(0).v, v);
	return _mm_sqrt_pd(radicand.v);
#else
	return Sample(BhMath::sqrt(v[0]), BhMath::sqrt(v[1]));
#endif
}

floatType Sample::avgValue() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	alignas(16) floatType lr[2];
	store_aligned(lr);
	return (lr[0] + lr[1]) / 2.;
#else
	return ((v[0] + v[1]) / 2.);
#endif
}

floatType Sample::maxValue() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	alignas(16) floatType lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[1] : lr[0];
#else
	return v[0] < v[1] ? v[1] : v[0];
#endif
}

floatType Sample::minValue() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	alignas(16) floatType lr[2];
	store_aligned(lr);
	return lr[0] < lr[1] ? lr[0] : lr[1];
#else
	return v[0] < v[1] ? v[0] : v[1];
#endif
}

void
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::replaceLeftChannel(Sample in)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_shuffle_pd(in.v, v, _MM_SHUFFLE2(1, 0));
#else
	this->v[0] = in.v[0];
#endif
}

void
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
Sample::replaceRightChannel(Sample in)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	v = _mm_shuffle_pd(v, in.v, _MM_SHUFFLE2(1, 0));
#else
	this->v[1] = in.v[1];
#endif
}

Sample 
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall 
#endif
Sample::flippedChannels() const
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	return _mm_shuffle_pd(v, v, _MM_SHUFFLE2(0, 1));
#else
	return Sample(this->v[1], this->v[0]);
#endif
}