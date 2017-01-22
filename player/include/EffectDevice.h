#pragma once

#include "Device.h"

class EffectBase;
class ParameterTrack;

class EffectDevice : public Device
{
public:
	EffectDevice(unsigned int blockSize, EffectBase& effect, ParameterTrack* parameterValues);

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition) override;

private:
	EffectBase& effect;
	ParameterTrack* parameterValues;
};