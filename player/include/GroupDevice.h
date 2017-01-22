#pragma once

#include "CombinedDevice.h"

class GroupDevice : public CombinedDevice
{
public:
	using CombinedDevice::CombinedDevice;

	Sample* process(MidiTrack* tracks, const Sample* input, unsigned int numberOfSamples) override;
};