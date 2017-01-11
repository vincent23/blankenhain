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
	gmInstrument(const char* name, unsigned int numberOfRegions, gmSoundRegion* regions);
	~gmInstrument();

	unsigned int numberOfRegions;
	gmSoundRegion* regions;

	bool isLoopable;
	const char* name;

	gmSoundRegion* findTargetRegion(unsigned int rootNote) const;
};
