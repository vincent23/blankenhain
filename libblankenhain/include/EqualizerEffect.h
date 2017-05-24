#pragma once

#include "EffectBase.h"

#include "Filter.h"

class EqualizerEffect : public EffectBase
{
public:
	EqualizerEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;

	const unsigned int numberOfFilters = 8;

private:
	Filter<Sample> filters[8];
};