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
public:
	// TODO remove last argument to constructor
	gmInstrument(unsigned int numberOfRegions, gmSoundRegion* regions, void* _);
	~gmInstrument();

	unsigned int numberOfRegions;
	gmSoundRegion* regions;

	bool isLoopable;

	gmSoundRegion* findTargetRegion(unsigned int rootNote) const;
};
