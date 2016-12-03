#pragma once

#include "EffectBase.h"

#include "Filter.h"

class FilterEffect : public EffectBase
{
public:
	FilterEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;

protected:
	Filter filters[4];
};