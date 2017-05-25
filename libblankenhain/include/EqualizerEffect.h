#pragma once

#include "EffectBase.h"

#include "Filter.h"

constexpr unsigned int numberOfEqualizerFilters = 8;

class EqualizerEffect : public EffectBase
{
public:
	EqualizerEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;



private:
	Filter<Sample> filters[8];
};