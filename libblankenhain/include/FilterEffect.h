#pragma once

#include "EffectBase.h"

#include "Filter.h"

class FilterEffect : public EffectBase
{
public:
	FilterEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;

protected:
	Filter<Sample> filters[4];
};