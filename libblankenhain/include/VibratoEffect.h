#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class VibratoEffect : public EffectBase
{
public:
	VibratoEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
    void resetDelayLine()
    {
      this->delayLine.reset();
    }
protected:
	bool wasPaniced;
	LinearInterpolatedCircularBuffer<Sample> delayLine;
};
