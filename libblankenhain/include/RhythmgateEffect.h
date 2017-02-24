#pragma once

#include "EffectBase.h"

class RhythmgateEffect : public EffectBase
{
public:
	RhythmgateEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime);
};