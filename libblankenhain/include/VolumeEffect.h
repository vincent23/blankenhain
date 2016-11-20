#pragma once

#include "EffectBase.h"

class VolumeEffect : public EffectBase
{
public:
	VolumeEffect();

	void process(Sample* buffer, size_t sampleFrames, size_t numberOfParameters, float* parameters) override;
};