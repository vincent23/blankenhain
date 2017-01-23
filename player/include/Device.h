#pragma once

#include "Sample.h"

class SongInfo;

class Device
{
public:
	Device();
	virtual ~Device();

	virtual Sample* process(SongInfo& songInfo, const Sample* input, unsigned int globalSamplePosition) = 0;

protected:
	Sample* outputBuffer;
};