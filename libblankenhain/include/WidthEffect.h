#pragma once

#include "EffectBase.h"

class WidthEffect : public EffectBase
{
public:
	WidthEffect();
	void process(Sample* buffer, size_t numberOfSamples) override;
};