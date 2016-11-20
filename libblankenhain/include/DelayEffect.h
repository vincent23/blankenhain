#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class DelayEffect : public EffectBase
{
public:
	DelayEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t numberOfParameters, float* parameters) override;

protected:
	CircularBuffer<Sample> delayLine;
};
