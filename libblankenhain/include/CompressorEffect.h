#pragma once

#include "EffectBase.h"

#include "CircularBuffer.h"
#include "EnvelopeFollower.h"

class CompressorEffect : public EffectBase
{
public:
	CompressorEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
	Sample getCurrentEnvelope() const;

	static double compressorGain(float threshold, float ratio, float knee, float dbIn);

private:
	EnvelopeFollower envelope;
	CircularBuffer<Sample> lookaheadDelay;
};