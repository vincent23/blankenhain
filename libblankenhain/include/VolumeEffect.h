#pragma once

#include "EffectBase.h"
#include "Oscillators.h"

class VolumeEffect : public EffectBase
{
public:
	VolumeEffect();
	virtual void getModulation(float* modulationValues, size_t sampleOffset = 0) override;

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;
private:
	PolyBLEPOscillator lfo;
};