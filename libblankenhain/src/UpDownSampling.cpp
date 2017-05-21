#include "UpDownSampling.h"
#include "Sample.h"

namespace spline
{
	// all via http://www.geos.ed.ac.uk/~yliu23/docs/lect_spline.pdf

	void solveTridiagonal(double* a, double* b, double* c, double* d, int n) {
		//via https://en.wikibooks.org/wiki/Algorithm_Implementation/Linear_Algebra/Tridiagonal_matrix_algorithm
		/*
		// n is the number of unknowns

		|b0 c0 0 ||x0| |d0|
		|a1 b1 c1||x1|=|d1|
		|0  a2 b2||x2| |d2|

		1st iteration: b0x0 + c0x1 = d0 -> x0 + (c0/b0)x1 = d0/b0 ->

		x0 + g0x1 = r0               where g0 = c0/b0        , r0 = d0/b0

		2nd iteration:     | a1x0 + b1x1   + c1x2 = d1
		from 1st it.: -| a1x0 + a1g0x1        = a1r0
		-----------------------------
		(b1 - a1g0)x1 + c1x2 = d1 - a1r0

		x1 + g1x2 = r1               where g1=c1/(b1 - a1g0) , r1 = (d1 - a1r0)/(b1 - a1g0)

		3rd iteration:      | a2x1 + b2x2   = d2
		from 2st it. : -| a2x1 + a2g1x2 = a2r2
		-----------------------
		(b2 - a2g1)x2 = d2 - a2r2
		x2 = r2                      where                     r2 = (d2 - a2r2)/(b2 - a2g1)
		Finally we have a triangular matrix:
		|1  g0 0 ||x0| |r0|
		|0  1  g1||x1|=|r1|
		|0  0  1 ||x2| |r2|

		Condition: ||bi|| > ||ai|| + ||ci||

		in this version the c matrix reused instead of g
		and             the d matrix reused instead of r and x matrices to report results
		Written by Keivan Moradi, 2014
		*/
		n--; // since we start from x0 (not x1)
		c[0] /= b[0];
		d[0] /= b[0];

		for (int i = 1; i < n; i++) {
			c[i] /= b[i] - a[i] * c[i - 1];
			d[i] = (d[i] - a[i] * d[i - 1]) / (b[i] - a[i] * c[i - 1]);
		}

		d[n] = (d[n] - a[n] * d[n - 1]) / (b[n] - a[n] * c[n - 1]);

		for (int i = n; i-- > 0;) {
			d[i] -= c[i] * d[i + 1];
		}
	}

	double B_0fkt(double x, double x0, double h)
	{
		if (x <= x0 - 2. *h) return 0.;
		else if (x <= x0 - h) return (1. / 6.) * pow((2 * h + (x - x0)), 3);
		else if (x <= x0) return (2 * h*h*h / 3) - 0.5*(x - x0)*(x - x0)*(2 * h + (x - x0));
		else if (x <= x0 + h) return (2 * h*h*h / 3) - 0.5*(x - x0)*(x - x0)*(2 * h - (x - x0));
		else if (x <= x0 + 2 * h) return (1. / 6.) * pow((2 * h - (x - x0)), 3);
		else return 0.;
	}

	double B_fkt(double k, double x, double x0, double h)
	{
		return B_0fkt(x - k*h + x0, x0, h);
	}
}

Sample* upsampleSpline(Sample* buffer, size_t originalSamples, size_t interpolatedSamples)
{
	using namespace spline;
	double* a, *b, *c, *d_left, *d_right;
	a = new double[originalSamples];
	b = new double[originalSamples];
	c = new double[originalSamples];
	d_left = new double[originalSamples];
	d_right = new double[originalSamples];

	// Store Left and Right Samples
	alignas(16) floatType tempbuffer[2];
	for (size_t i = 0; i < originalSamples; i++)
	{
		buffer[i].store_aligned(tempbuffer);
		d_left[i] = tempbuffer[0];
		d_right[i] = tempbuffer[1];
	}
	a[0] = 0.;
	b[0] = 1.;
	c[0] = 1.;
	a[originalSamples - 1u] = 0.;
	b[originalSamples - 1u] = 1.;
	c[originalSamples - 1u] = 1.;
	for (size_t i = 1; i < originalSamples - 1; i++)
	{
		a[i] = 1.;
		c[i] = 1.;
		b[i] = 4.;
	}

	// Get left spine coefficients
	solveTridiagonal(a, b, c, d_left, originalSamples);
	solveTridiagonal(a, b, c, d_right, originalSamples);
	delete[] a;
	delete[] b;
	delete[] c;

	//////////////////////////////////////////
	//////////////////////////////////////////

	// Allocate new (to be interpolated) buffer
	Sample* newBuffer = new Sample[interpolatedSamples];
	float stepForeward = static_cast<float>(originalSamples - 1) / static_cast<float>(interpolatedSamples);

	// Special Cases: Beginning & Ending
	newBuffer[0] = buffer[0];
	newBuffer[interpolatedSamples - 1] = buffer[originalSamples - 1];
	size_t currentPositionInInterpolatedArray = 1;

	// Regular case
	for (float currentPosition = stepForeward;
		currentPosition < static_cast<float>(originalSamples - 1);
		currentPosition += stepForeward, currentPositionInInterpolatedArray++)
	{
		size_t j = static_cast<int>(currentPosition);
		double leftvalue, rightvalue;
		if (j == 0)
		{
			leftvalue = (2 * d_left[j] - d_left[j + 1]) * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_left[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_left[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ d_left[j + 2] * B_fkt(j + 2, currentPosition, 0., 1.);
			rightvalue = (2 * d_right[j] - d_right[j + 1]) * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_right[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_right[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ d_right[j + 2] * B_fkt(j + 2, currentPosition, 0., 1.);
		}
		else if (j == originalSamples - 2u)
		{
			leftvalue = d_left[j - 1] * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_left[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_left[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ (2 * d_left[j] - d_left[j - 1]) * B_fkt(j + 2, currentPosition, 0., 1.);
			rightvalue = d_right[j - 1] * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_right[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_right[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ (2 * d_right[j] - d_right[j - 1]) * B_fkt(j + 2, currentPosition, 0., 1.);
		}
		else
		{
			leftvalue = d_left[j - 1] * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_left[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_left[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ d_left[j + 2] * B_fkt(j + 2, currentPosition, 0., 1.);
			rightvalue = d_right[j - 1] * B_fkt(j - 1, currentPosition, 0., 1.) \
				+ d_right[j] * B_fkt(j, currentPosition, 0., 1.) \
				+ d_right[j + 1] * B_fkt(j + 1, currentPosition, 0., 1.) \
				+ d_right[j + 2] * B_fkt(j + 2, currentPosition, 0., 1.);
		}
		newBuffer[currentPositionInInterpolatedArray] = Sample(leftvalue, rightvalue);
	}

	delete[] d_left;
	delete[] d_right;
	return newBuffer;
}

Sample* simpleDownsample(Sample* buffer, size_t originalSamples, size_t divisor)
{
	//Check
	size_t divisorCopy = divisor;
	while (divisorCopy != 1)
	{
		if (divisorCopy % 2 != 0) exit(5);
		divisorCopy /= 2;
	}

	Sample* out = new Sample[originalSamples / divisor];
	for (size_t i = 0; i < originalSamples / divisor; i++)
	{
		out[i] = buffer[i*divisor];
	}

	return out;
}

Sample* upsampleZeros(Sample* buffer, size_t originalSamples, size_t multiplier)
{
	// Allocate new (to be interpolated) buffer
	Sample* newBuffer = new Sample[multiplier * originalSamples];

	for (size_t i = 0u; i < originalSamples; i++)
	{
		newBuffer[i*multiplier] = buffer[i];
		for (size_t j = 1; j < multiplier; j++) newBuffer[i*multiplier + j] = Sample(0.);
	}

	return newBuffer;
}
