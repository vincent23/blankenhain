#pragma once

#include "Sample.h"

class MidiTrack;

class Device
{
public:
	Device(unsigned int blockSize);
	virtual ~Device();

	virtual Sample* process(MidiTrack* tracks, const Sample* input, unsigned int numberOfSamples) = 0;

protected:
	Sample* outputBuffer;
};