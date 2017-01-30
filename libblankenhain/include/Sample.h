#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>
#include "AlignedType.h"

/**
 * A stereo sample.
 */
struct alignas(16) Sample : public AlignedType
{
	/**
	 * Left and right volume two doubles packed in a SSE register.
	 */
	Sample() : v(_mm_set1_pd(0.)) {}
	explicit Sample(const double singleSample) { v = _mm_set1_pd(singleSample); }
	Sample(__m128d in) { v = in; }
	Sample(const Sample& x) { v = x.v; }
	Sample(double sampleLeft, double sampleRight) { v = _mm_set_pd(sampleLeft, sampleRight); }
	alignas(16) __m128d v;
	inline __m128d& get(void)
	{
		return v;
	}

	Sample& operator=(const Sample& other)
	{
		v = other.v;
		return *this;
	}

	Sample& operator+=(const Sample& other)
	{
		v = _mm_add_pd(v, other.v);
		return *this;
	}

	Sample& operator-=(const Sample& other)
	{
		v = _mm_sub_pd(v, other.v);
		return *this;
	}

	Sample& operator*=(const Sample& other)
	{
		v = _mm_mul_pd(v, other.v); return *this;
	}

	Sample& operator/=(const Sample& other)
	{
		v = _mm_div_pd(v, other.v); return *this;
	}

	/**
	 * Creates a sample value from two double values at the given address.
	 * Note that the address must be aligned to 16-byte boundaries.
	 * @param ptr The location of the sample values.
	 * @return The values as a Sample.
	 */
	 //static
	void load_aligned(const double* ptr)
	{
		v = _mm_load_pd(ptr);
	}

	/**
	 * Write the sample values as two doubles to the given address.
	 * Note that the address must be aligned to 16-byte boundaries.
	 * @param The location to which the values should be written.
	 */
	void store_aligned(double* ptr) const
	{
		_mm_store_pd(ptr, v);
	}

	void store_unaligned(double* ptr) const
	{
		alignas(16) double tmp[2];
		_mm_store_pd(tmp, v);
		ptr[0] = tmp[0];
		ptr[1] = tmp[1];
	}

	bool areBothSmaller(Sample const& in)
	{
		alignas(16) double in_[2], this_[2];
		in.store_aligned(in_);
		this->store_aligned(this_);
		return this_[0] < in_[0] && this_[1] < in_[1];
	}
};

inline Sample operator+(const Sample& a, const Sample& b) {
	return _mm_add_pd(a.v, b.v);
}

inline Sample operator-(const Sample& a, const Sample& b) {
	return _mm_sub_pd(a.v, b.v);
}

inline Sample operator*(const Sample& a, const Sample& b) {
	return _mm_mul_pd(a.v, b.v);
}

inline Sample operator/(const Sample& a, const Sample& b) {
	return _mm_div_pd(a.v, b.v);
}

static Sample operator-(const Sample& a)
{
	// flip sign bit
	return _mm_xor_pd(a.v, Sample(-0.f).v);
}

inline Sample load_aligned(const double* ptr)
{
	return _mm_load_pd(ptr);
}

static Sample abs(const Sample& in)
{
	// delete sign bits
	return _mm_andnot_pd(Sample(-0.f).v, in.v);
}

static Sample sign(Sample const& in)
{
	// not properly tested, could be wrong
	// extract sign bit, then combine it with 1
	__m128d signBits = _mm_and_pd(in.v, Sample(-0.f).v);
	return _mm_or_pd(signBits, Sample(1).v);
}

inline double avgValue(Sample const& in)
{
	alignas(16) double lr[2];
	in.store_aligned(lr);
	return (lr[0] + lr[1]) / 2.;
}

inline double maxValue(Sample const& in)
{
	alignas(16) double lr[2];
	in.store_aligned(lr);
	return lr[0] < lr[1] ? lr[1] : lr[0];
}

inline double minValue(Sample const& in)
{
	alignas(16) double lr[2];
	in.store_aligned(lr);
	return lr[0] < lr[1] ? lr[0] : lr[1];
}

inline Sample sqrt(const Sample& in)
{
	return _mm_sqrt_pd(in.v);
}