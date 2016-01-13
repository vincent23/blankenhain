#pragma once

#include "Sample.h"
#include "Filter.h"

class KickSynth : public AlignedType
{
public:
	KickSynth();

	void kick();
	void play(Sample* data, size_t numberOfSamples);

private:
	size_t samplesSinceKick = 0;
	// TODO: replace cascaded first order filters
	Filter high[3];
	Filter low[3];
	Filter lowHarmonic;
};