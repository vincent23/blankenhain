#pragma once

#include "EffectBase.h"
#include "Oscillators.h"
#include "Filter.h"

class FilterEffect : public EffectBase
{
public:
	FilterEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
	virtual void getModulation(float* modulationValues, size_t sampleOffset = 0) override;

	CommonLFO& getLFO()
	{
		return lfo;
	}

protected:
	Filter<Sample> filters[4];
	CommonLFO lfo;
};