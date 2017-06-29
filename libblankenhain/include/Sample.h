#pragma once
#include "Options.h"

#pragma warning(disable: 4324)
#include <emmintrin.h>


#ifdef _LIBBLANKENHAIN_NO_SSE
#include "BhMath.h"

#ifdef _LIBBLANKENHAIN_USE_SINGE_PRECISION
typedef float floatType;
#endif

#else
typedef double floatType;
#endif

/**
 * A stereo sample.
 */
class Sample
{
public:
	/**
	 * Left and right volume two doubles packed in a SSE register.
	 */
	Sample()
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

	explicit Sample(floatType singleSample)
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		v = _mm_set1_pd(singleSample);
#else
		v[0] = singleSample;
		v[1] = singleSample;
#endif
	}

#ifndef _LIBBLANKENHAIN_NO_SSE
	Sample(__m128d in)
	{
		v = in;
	}
#endif

	Sample(floatType sampleLeft, floatType sampleRight)
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		v = _mm_set_pd(sampleRight, sampleLeft);
#else
		v[0] = sampleLeft;
		v[1] = sampleRight;
#endif
	}

#ifndef _LIBBLANKENHAIN_NO_SSE
	static Sample _vectorcall load_aligned(const floatType* ptr)
	{
		return _mm_load_pd(ptr);
	}
#endif

	/**
	* Creates a sample value from two floatType values at the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param ptr The location of the sample values.
	* @return The values as a Sample.
	*/


	// Sample can't derive AlignedType in order for vectorcall to work
	void *operator new[](unsigned int size);
	void operator delete[](void *p);

	/**
	* Write the sample values as two floatTypes to the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param The location to which the values should be written.
	*/
	// Todo Remove all calls to this
	void store_aligned(floatType* ptr) const
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
		operator+=(Sample other)
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
		operator-=(Sample other)
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
		operator*=(Sample other)
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
		operator/=(Sample other)
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
		abs() const
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
		sign() const
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
		sqrt() const
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		Sample radicand = _mm_max_pd(Sample(0).v, v);
		return _mm_sqrt_pd(radicand.v);
#else
		return Sample(BhMath::sqrt(v[0]), BhMath::sqrt(v[1]));
#endif
	}

	void
#ifndef _LIBBLANKENHAIN_NO_SSE
		__vectorcall
#endif
		replaceLeftChannel(Sample in)
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
		replaceRightChannel(Sample in)
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		v = _mm_shuffle_pd(v, in.v, _MM_SHUFFLE2(1, 0));
#else
		this->v[1] = in.v[1];
#endif
	}

	Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
		_vectorcall
#endif
		flippedChannels() const
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		return _mm_shuffle_pd(v, v, _MM_SHUFFLE2(0, 1));
#else
		return Sample(this->v[1], this->v[0]);
#endif
	}


	floatType avgValue() const
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		alignas(16) floatType lr[2];
		store_aligned(lr);
		return (lr[0] + lr[1]) / 2.;
#else
		return ((v[0] + v[1]) / 2.);
#endif
	}

	floatType maxValue() const
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		alignas(16) floatType lr[2];
		store_aligned(lr);
		return lr[0] < lr[1] ? lr[1] : lr[0];
#else
		return v[0] < v[1] ? v[1] : v[0];
#endif
	}

	floatType minValue() const
	{
#ifndef _LIBBLANKENHAIN_NO_SSE
		alignas(16) floatType lr[2];
		store_aligned(lr);
		return lr[0] < lr[1] ? lr[0] : lr[1];
#else
		return v[0] < v[1] ? v[0] : v[1];
#endif
	}


#ifndef _LIBBLANKENHAIN_NO_SSE
	__m128d v;
#else
	float v[2];
#endif
};

inline Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
operator+(Sample a, Sample b)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	return _mm_add_pd(a.v, b.v);
#else
	return Sample(a.v[0] + b.v[0], a.v[1] + b.v[1]);
#endif
}

inline Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
operator-(Sample a, Sample b)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	return _mm_sub_pd(a.v, b.v);
#else
	return Sample(a.v[0] - b.v[0], a.v[1] - b.v[1]);
#endif
}

inline Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
operator*(Sample a, Sample b)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	return _mm_mul_pd(a.v, b.v);
#else
	return Sample(a.v[0] * b.v[0], a.v[1] * b.v[1]);
#endif
}

inline Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
operator/(Sample a, Sample b)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	return _mm_div_pd(a.v, b.v);
#else
	return Sample(a.v[0] / b.v[0], a.v[1] / b.v[1]);
#endif
}

inline Sample
#ifndef _LIBBLANKENHAIN_NO_SSE
__vectorcall
#endif
operator-(Sample a)
{
#ifndef _LIBBLANKENHAIN_NO_SSE
	// flip sign bit
	return _mm_xor_pd(a.v, Sample(-0.f).v);
#else
	return Sample(-a.v[0], -a.v[1]);
#endif
}

