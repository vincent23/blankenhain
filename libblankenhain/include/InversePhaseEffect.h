#pragma once

#include "EffectBase.h"
#include "ParameterBundle.h"
#include "AuxFunc.h"
#include "InterpolatedValue.h"

class InversePhaseEffect : public EffectBase
{
public:
	InversePhaseEffect();
	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) final;
};