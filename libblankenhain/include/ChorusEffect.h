#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"
#include "Oscillators.h"
class ChorusEffect : public EffectBase
{
public:
	ChorusEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
    void resetDelayLine()
    {
      this->delayLine.reset();
    }

	CommonLFO& getLFO()
	{
		return lfo;
	}
protected:
	bool wasPaniced;
	CircularBuffer<Sample> delayLine;
	CommonLFO lfo;
};
