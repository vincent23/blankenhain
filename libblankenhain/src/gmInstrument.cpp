#include "gmInstrument.h"


gmInstrument::gmInstrument(const char* name_, unsigned int numberOfRegions_, gmSoundRegion* regions_)
	: numberOfRegions(numberOfRegions_), regions(regions_), isLoopable(false), name(name_)
{
	if (regions[0].isLoopable == true)
		isLoopable = true;
	this->regions = new gmSoundRegion[numberOfRegions_];
	for (unsigned int i = 0u; i < numberOfRegions_; i++)
		this->regions[i] = regions_[i];
}

gmInstrument::~gmInstrument()
{
	if (regions != nullptr)
		delete[] regions;
	regions = nullptr;
}

gmSoundRegion* gmInstrument::findTargetRegion(unsigned int rootNote) const
{
	for (unsigned int i = 0u; i < numberOfRegions; i++) {
		if (rootNote >= regions[i].lowest && rootNote <= regions[i].highest) {
			return &(regions[i]);
		}
	}
	return nullptr;
}