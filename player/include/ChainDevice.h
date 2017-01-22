#pragma once

#include "CombinedDevice.h"

class ChainDevice : public CombinedDevice
{
public:
	using CombinedDevice::CombinedDevice;

	Sample* process(MidiTrack* tracks, const Sample* input, unsigned int numberOfSamples) override;
};