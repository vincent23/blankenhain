#pragma once

#include "Sample.h"

// algorithm from https://cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf
// TODO we use the bounded version and could compare performance of the unbounded

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

	Sample tick(const Sample& v0);
private:
	void recomputeCoefficients(double frequency, double Q, double gFactor);

	Sample g, k, a1, a2, a3;
	Sample m0, m1, m2;
	Sample ic1eq, ic2eq;
	Sample v1, v2, v3;
};