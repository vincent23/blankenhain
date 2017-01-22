#pragma once

#include "Sample.h"

class SongInfo;

class Device
{
public:
	Device(unsigned int blockSize);
	virtual ~Device();

	virtual Sample* process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition) = 0;

protected:
	Sample* outputBuffer;
};