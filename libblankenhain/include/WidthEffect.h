#pragma once

#include "EffectBase.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"
#include "Oscillators.h"

class WidthEffect : public EffectBase
{
public:
	WidthEffect();
	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) final;
	void getModulation(float* modulationValues, size_t sampleOffset) final;

private:
	PolyBLEPOscillator lfo;
};