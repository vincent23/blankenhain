#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class DelayEffect : public EffectBase
{
public:
	DelayEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
  void resetDelayLine()
  {
    this->delayLine.reset();
  }
protected:
	bool wasPaniced;
	LinearInterpolatedCircularBuffer<Sample> delayLine;
};
