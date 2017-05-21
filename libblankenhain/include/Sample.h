#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>

#define _LIBBLANKENHAIN_NO_SSE
#define _LIBBLANKENHAIN_USE_SINGE_PRECISION

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
	Sample();
	explicit Sample(floatType singleSample);
#ifndef _LIBBLANKENHAIN_NO_SSE
	Sample(__m128d in);
	static Sample _vectorcall load_aligned(const floatType* ptr);
#endif
	Sample(floatType sampleLeft, floatType sampleRight);
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
	void store_aligned(floatType* ptr) const;

#ifndef _LIBBLANKENHAIN_NO_SSE
	Sample _vectorcall operator+=(Sample other);
	Sample _vectorcall operator-=(Sample other);
	Sample _vectorcall operator*=(Sample other);
	Sample _vectorcall operator/=(Sample other);

	Sample _vectorcall abs() const;
	Sample _vectorcall sign() const;
	Sample _vectorcall sqrt() const;

	void _vectorcall replaceLeftChannel(Sample in);
	void _vectorcall replaceRightChannel(Sample in);
	Sample __vectorcall flippedChannels() const;
#else
	Sample operator+=(Sample other);
	Sample operator-=(Sample other);
	Sample operator*=(Sample other);
	Sample operator/=(Sample other);

	Sample abs() const;
	Sample sign() const;
	Sample sqrt() const;

	void replaceLeftChannel(Sample in);
	void replaceRightChannel(Sample in);
	Sample flippedChannels() const;
#endif

	floatType avgValue() const;
	floatType maxValue() const;
	floatType minValue() const;


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

