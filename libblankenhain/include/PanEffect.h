#pragma once

#include "EffectBase.h"

class PanEffect : public EffectBase
{
public:
	PanEffect();

	void process(Sample* buffer, size_t numberOfSamples) override;
};