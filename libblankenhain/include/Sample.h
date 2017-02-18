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
	static Sample _vectorcall load_aligned(const double* ptr);

	/**
	* Write the sample values as two doubles to the given address.
	* Note that the address must be aligned to 16-byte boundaries.
	* @param The location to which the values should be written.
	*/
	void _vectorcall store_aligned(double* ptr) const;

	Sample& _vectorcall operator=(const Sample& other);
	Sample& _vectorcall operator+=(const Sample& other);
	Sample& _vectorcall operator-=(const Sample& other);
	Sample& _vectorcall operator*=(const Sample& other);
	Sample& _vectorcall operator/=(const Sample& other);

	Sample _vectorcall operator+(const Sample& other) const;
	Sample _vectorcall operator-(const Sample& other) const;
	Sample _vectorcall operator*(const Sample& b) const;
	Sample _vectorcall operator/(const Sample& other) const;
	Sample _vectorcall operator-() const;

	Sample _vectorcall abs() const;
	Sample _vectorcall sign() const;
	Sample _vectorcall sqrt() const;

	double _vectorcall avgValue() const;
	double _vectorcall maxValue() const;
	double _vectorcall minValue() const;

private:
	__m128d v;
};