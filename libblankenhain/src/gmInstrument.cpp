#include "gmInstrument.h"

#include "gmSound.h"

gmInstrument::gmInstrument(unsigned int numberOfRegions_, gmSoundRegion* regions_, HANDLE handle_)
	: numberOfRegions(numberOfRegions_), regions(regions_), mIsLoopable(false), interpolatedSounds(nullptr)
{
	if (regions[0].isLoopable == true)
		mIsLoopable = true;
	this->regions = new gmSoundRegion[numberOfRegions_];
	for (unsigned int i = 0u; i < numberOfRegions_; i++)
		this->regions[i] = regions_[i];

	interpolatedSounds = new gmSound*[128];
	for (unsigned int i = 0u; i < 128; i++)
	{
		interpolatedSounds[i] = new gmSound(findTargetRegion(i), handle_, i);
	}
}

gmInstrument::~gmInstrument()
{
	if (regions != nullptr)
		delete[] regions;
	regions = nullptr;

	if (interpolatedSounds != nullptr)
		for (unsigned int i = 0u; i < 128; i++)
		{
			delete interpolatedSounds[i];
			interpolatedSounds[i] = nullptr;
		}
	delete[] interpolatedSounds;
}

void gmInstrument::getNote(unsigned int targetNote, Sample*& returnedBuffer, unsigned int& returnedBufferSize) const
{
	returnedBufferSize = (this->interpolatedSounds[targetNote])->interpolatedBufferSize;
	returnedBuffer = (this->interpolatedSounds[targetNote])->interpolatedBuffer;
}

bool gmInstrument::isLoopable() const
{
	return mIsLoopable;
}

gmSoundRegion gmInstrument::findTargetRegion(unsigned int rootNote) const
{
	for (unsigned int i = 0u; i < numberOfRegions; i++)
		if (rootNote >= regions[i].lowest && rootNote <= regions[i].highest)
			return regions[i];
	return gmSoundRegion();
}