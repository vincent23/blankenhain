#pragma once

#include "EffectDevice.h"

class InstrumentBase;

class InstrumentDevice : public EffectDevice
{
public:
	InstrumentDevice(unsigned int blockSize, InstrumentBase& instrument, ParameterTrack* parameterValues, unsigned int inputTrackIndex);

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition) override;

private:
	InstrumentBase& getInstrument();
	unsigned int inputTrackIndex;
};