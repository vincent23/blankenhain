#pragma once

#include "Device.h"

class EffectBase;
class ParameterTrack;

class EffectDevice : public Device
{
public:
	EffectDevice(EffectBase& effect, ParameterTrack* parameterValues);

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition) override;

protected:
	EffectBase& effect;

private:
	ParameterTrack* parameterValues;
};