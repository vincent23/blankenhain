#pragma once

#include "EffectBase.h"

#include "processFunctions.h"

class EqualizerEffect : public EffectBase
{
public:
	EqualizerEffect();
	~EqualizerEffect();

	void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters) override;

private:
	effects::equalizer::Sample_EQSTATE* es;
	const double vsa = (1.0 / 4294967295.0); // Very small amount (Denormal Fix)
};