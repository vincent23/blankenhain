#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"
#include "Oscillators.h"
class ChorusEffect : public EffectBase
{
public:
	ChorusEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
    void resetDelayLine()
    {
      this->delayLine.reset();
    }
protected:
	bool wasPaniced;
	LinearInterpolatedCircularBuffer<Sample> delayLine;
	PolyBLEPOscillator lfo;
};
