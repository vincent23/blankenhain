#pragma once

#include "EffectBase.h"

#include "CircularBuffer.h"
#include "EnvelopeFollower.h"

class CompressorEffect : public EffectBase
{
public:
	CompressorEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
	Sample getCurrentEnvelope() const;

	static double compressorGain(double threshold, double ratio, double knee, double dbIn);
private:
	EnvelopeFollower envelope;
	CircularBuffer<Sample> lookaheadDelay;
};