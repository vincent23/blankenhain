#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class ExampleEffect : public EffectBase
{
public:
	// Change the name and define parameters in constructor
	ExampleEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;

protected:
	// Include optional additional stuff
	CircularBuffer<Sample> delayLine;
	Sample envelope;
};