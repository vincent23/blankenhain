#pragma once

#include "EffectBase.h"

#include "CircularBuffer.h"

class CompressorEffect : public EffectBase
{
public:
	CompressorEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
};