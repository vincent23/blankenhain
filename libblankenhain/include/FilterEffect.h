#pragma once

#include "EffectBase.h"

#include "processFunctions.h"

class FilterEffect : public EffectBase
{
public:
	FilterEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;

protected:
	effects::filter::Filter filter;
};