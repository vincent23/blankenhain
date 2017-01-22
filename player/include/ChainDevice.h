#pragma once

#include "CombinedDevice.h"

class ChainDevice : public CombinedDevice
{
public:
	using CombinedDevice::CombinedDevice;

	Sample* process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition) override;
};