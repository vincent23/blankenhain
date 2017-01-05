#pragma once

#include "gmSoundRegion.h"
class gmSound;

using HANDLE = void*;

struct Sample;

/**
* Stores interpolated sounds of one gmInstrument in a gmSound array.
* Access via function getNote().
*/
class gmInstrument
{
private:
	// used
	unsigned int numberOfRegions;
	gmSoundRegion* regions;

	bool mIsLoopable;
	//HANDLE* h;

	gmSound** interpolatedSounds;

	//works
	gmSoundRegion findTargetRegion(unsigned int rootNote) const;
public:
	gmInstrument(unsigned int numberOfRegions, gmSoundRegion* regions, HANDLE handle);
	~gmInstrument();
	bool isLoopable() const;
	void getNote(unsigned int targetNote, Sample*& returnedBuffer, unsigned int& returnedBufferSize) const;
};
