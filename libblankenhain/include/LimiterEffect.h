#pragma once

#include "EffectBase.h"

#include "CircularBuffer.h"
#include "EnvelopeFollower.h"

class LimiterEffect : public EffectBase
{
public:
	LimiterEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
	Sample getCurrentEnvelope() const;

private:
	EnvelopeFollower envelope;
	CircularBuffer<Sample> lookaheadDelay;
};