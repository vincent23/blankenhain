#pragma once

#include "EffectBase.h"
#include "CircularBuffer.h"

class NoiseGateEffect : public EffectBase
{
public:
	NoiseGateEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
private:
	CircularBuffer<Sample> lookaheadBuffer;
	Sample RMSkeeper;
	const unsigned int RMSVolLength;
	unsigned int lastStartTime;
	unsigned int lastStopTime;
};