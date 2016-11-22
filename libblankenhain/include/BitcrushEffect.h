#pragma once

#include "EffectBase.h"

class BitcrushEffect : public EffectBase
{
public:
	BitcrushEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
	Sample discretize(Sample const& sample);
};