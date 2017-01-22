#pragma once

#include "Sample.h"

class MidiTrack;

class Device
{
public:
	Device(unsigned int blockSize);

	virtual Sample* process(MidiTrack* tracks, Sample* input, unsigned int numberOfSamples) = 0;

protected:
	~Device();

	Sample* outputBuffer;
};