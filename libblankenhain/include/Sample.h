#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>

#include "AlignedType.h"

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
	explicit Sample(double singleSample);
	Sample(__m128d in);
	Sample(double sampleLeft, double sampleRight);
	/**
	* Creates a sample value from two double values at the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param ptr The location of the sample values.
	* @return The values as a Sample.
	*/
	static Sample _vectorcall load_aligned(const double* ptr);

	// Sample can't derive AlignedType in order for vectorcall to work
	void *operator new[](unsigned int size);
	void operator delete[](void *p);

	/**
	* Write the sample values as two doubles to the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param The location to which the values should be written.
	*/
	void store_aligned(double* ptr) const;

	Sample _vectorcall operator+=(Sample other);
	Sample _vectorcall operator-=(Sample other);
	Sample _vectorcall operator*=(Sample other);
	Sample _vectorcall operator/=(Sample other);

	Sample _vectorcall abs() const;
	Sample _vectorcall sign() const;
	Sample _vectorcall sqrt() const;

	double avgValue() const;
	double maxValue() const;
	double minValue() const;

	void _vectorcall replaceLeftChannel(Sample in);
	void _vectorcall replaceRightChannel(Sample in);
	Sample __vectorcall flippedChannels() const;


#ifndef _LIBBLANKENHAIN_NO_SSE
	__m128d v;
#else
	double v
#endif
};

inline Sample _vectorcall operator+(Sample a, Sample b)
{
	return _mm_add_pd(a.v, b.v);
}

inline Sample _vectorcall operator-(Sample a, Sample b)
{
	return _mm_sub_pd(a.v, b.v);
}

inline Sample _vectorcall operator*(Sample a, Sample b)
{
	return _mm_mul_pd(a.v, b.v);
}

inline Sample _vectorcall operator/(Sample a, Sample b)
{
	return _mm_div_pd(a.v, b.v);
}

inline Sample _vectorcall operator-(Sample a)
{
	// flip sign bit
	return _mm_xor_pd(a.v, Sample(-0.f).v);
}

