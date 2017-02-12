#pragma once
#pragma warning(disable: 4324)
#include <emmintrin.h>
#include "AlignedType.h"

/**
 * A stereo sample.
 */
class alignas(16) Sample : public AlignedType
{
public:
	/**
	 * Left and right volume two doubles packed in a SSE register.
	 */
	Sample();
	explicit Sample(double singleSample);
	Sample(__m128d in);
	Sample(const Sample& x);
	Sample(double sampleLeft, double sampleRight);
	/**
	* Creates a sample value from two double values at the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param ptr The location of the sample values.
	* @return The values as a Sample.
	*/
	static Sample load_aligned(const double* ptr);

	/**
	* Write the sample values as two doubles to the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param The location to which the values should be written.
	*/
	void store_aligned(double* ptr) const;

	Sample& operator=(const Sample& other);
	Sample& operator+=(const Sample& other);
	Sample& operator-=(const Sample& other);
	Sample& operator*=(const Sample& other);
	Sample& operator/=(const Sample& other);

	Sample operator+(const Sample& other) const;
	Sample operator-(const Sample& other) const;
	Sample operator*(const Sample& b) const;
	Sample operator/(const Sample& other) const;
	Sample operator-() const;

	Sample abs() const;
	Sample sign() const;
	Sample sqrt() const;

	double avgValue() const;
	double maxValue() const;
	double minValue() const;

private:
	alignas(16) __m128d v;
};