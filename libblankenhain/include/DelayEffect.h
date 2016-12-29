#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class DelayEffect : public EffectBase
{
public:
	DelayEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;

protected:
	bool wasPaniced;
	LinearInterpolatedCircularBuffer<Sample> delayLine;
};
