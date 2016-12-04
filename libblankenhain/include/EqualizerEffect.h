#pragma once

#include "EffectBase.h"

#include "Filter.h"

class EqualizerEffect : public EffectBase
{
public:
	EqualizerEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;

	static const unsigned int numberOfFilters = 8;

private:
	Filter filters[8];
};