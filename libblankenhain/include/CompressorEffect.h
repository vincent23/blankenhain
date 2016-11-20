#pragma once

#include "EffectBase.h"

#include "CircularBuffer.h"

const unsigned int NUMBER_OF_PARAMETERS = 5u;

class CompressorEffect : public EffectBase
{
public:
	CompressorEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters) override;

protected:
	CircularBuffer<Sample> delayLine;
	Sample envelope;
};