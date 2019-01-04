#pragma once

#include "EffectBase.h"

class BitcrushEffect : public EffectBase
{
public:
	BitcrushEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
	Sample discretize(Sample const& sample);

private:
	int discardedSamples = 100;
	Sample lastSample1;
	Sample lastSample2;
};