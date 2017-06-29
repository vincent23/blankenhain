#pragma once

#include "EffectBase.h"

class AbletonTrackMixerVolumeEffect : public EffectBase
{
public:
	AbletonTrackMixerVolumeEffect();

	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) override;

};