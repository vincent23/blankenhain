#pragma once

#include "Sample.h"

// algorithm from https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
// TODO we use the bounded version and could compare performance of the unbounded

class Filter : AlignedType
{
public:
	void recomputeCoefficients(double frequency, double Q);
	Sample tickLow(const Sample& in);
	Sample tickHigh(const Sample& in);
	Sample tickBand(const Sample& in);
	Sample tickNotch(const Sample& in);
	Sample tickPeak(const Sample& in);
	Sample tickAll(const Sample& in);
	
	// TODO bell, shelf?

private:
	void tick(const Sample& v0);

	double g, k, a1, a2, a3;
	Sample ic1eq, ic2eq;
	Sample v1, v2, v3;
};