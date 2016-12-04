#pragma once

#include "Sample.h"

// algorithm from https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
// TODO we use the bounded version and could compare performance of the unbounded

class Filter : AlignedType
{
public:
	void recomputeCoefficients(double frequency, double Q);
	void recomputeCoefficientsBell(double frequency, double Q, double gainInDb);
	void recomputeCoefficientsLowShelf(double frequency, double Q, double gainInDb);
	void recomputeCoefficientsHighShelf(double frequency, double Q, double gainInDb);
	Sample tickLow(const Sample& in);
	Sample tickHigh(const Sample& in);
	Sample tickBand(const Sample& in);
	Sample tickNotch(const Sample& in);
	Sample tickBell(const Sample& in);
	Sample tickLowShelf(const Sample& in);
	Sample tickHighShelf(const Sample& in);

	// TODO do we need these?
	Sample tickPeak(const Sample& in);
	Sample tickAll(const Sample& in);

private:
	void tick(const Sample& v0);
	void recomputeCoefficientsInternal(double frequency, double Q, double gFactor);

	Sample g, k, a1, a2, a3;
	Sample m0, m1, m2;
	Sample ic1eq, ic2eq;
	Sample v1, v2, v3;
};