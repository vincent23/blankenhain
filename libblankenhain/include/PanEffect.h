#pragma once

#include "EffectBase.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"
#include "Oscillators.h"

class PanEffect : public EffectBase
{
public:
	PanEffect();
	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) final;
	void getModulation(float* modulationValues, size_t sampleOffset) final;
	CommonLFO& getLFO()
	{
		return lfo;
	}
private:
	CommonLFO lfo;
};