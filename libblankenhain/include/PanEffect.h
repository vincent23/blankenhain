#pragma once

#include "EffectBase.h"

class PanEffect : public EffectBase
{
public:
	PanEffect();

	void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters) override;
};