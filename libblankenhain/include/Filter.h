#pragma once

#include "Sample.h"

// algorithm from https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
// TODO we use the bounded version and could compare performance of the unbounded

/**
 * Base class to perform filtering (lowpass / Highpass etc.)
 * Very old code, back from bh1
 */
template <typename T>
class Filter : AlignedType
{
public:
	void setLowPass(double frequency, double Q);
	void setHighPass(double frequency, double Q);
	void setBandPass(double frequency, double Q);
	void setNotch(double frequency, double Q);
	void setBell(double frequency, double Q, double gainInDb);
	void setLowShelf(double frequency, double Q, double gainInDb);
	void setHighShelf(double frequency, double Q, double gainInDb);
	// TODO do we need these?
	void setPeak(double frequency, double Q);
	void setAll(double frequency, double Q);

	T tick(const T& v0);
private:
	void recomputeCoefficients(double frequency, double Q, double gFactor);

	T g, k, a1, a2, a3;
	T m0, m1, m2;
	T ic1eq, ic2eq;
	T v1, v2, v3;
};