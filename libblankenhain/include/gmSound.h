#pragma once
#include "Options.h"
#include "gmSoundRegion.h"

using HANDLE = void*;

class Sample;

/**
* Contains a note of a gmInstrument as a Sample array.
* The constructor performs interpolation of the sample found based
* on the gmSoundRegion handed over during construction.
*/
class gmSound
{
public:
	gmSound(gmSoundRegion& region, HANDLE h, unsigned int targetNote);
	~gmSound();

	float* interpolatedBuffer;
	unsigned int interpolatedBufferSize;
	bool isLoopable;
	unsigned int loopStart;
	unsigned int loopLength;
private:

};