#pragma once

#include "Sample.h"
#include "processFunctions.h"

using namespace effects::filter;

class KickSynth : public AlignedType
{
public:
	KickSynth();

	void kick();
	void play(Sample* data, size_t numberOfSamples);

private:
  Filter high[3];
  Filter low[3];
  Filter lowHarmonic;
	size_t samplesSinceKick = 0;
	// TODO: replace cascaded first order filters


};