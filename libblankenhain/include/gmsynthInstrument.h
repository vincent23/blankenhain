#pragma once

#include "InstrumentBase.h"
#include <windows.h>

class VoiceState;
class gmSound;

/**
* Region data via dlParser. Only stores information, no processing here.
*/
class gmSoundRegion
{
public:
	unsigned int rootNote;
	unsigned int lowest;
	unsigned int highest;
	unsigned int startByte;
	// These three are in Byte! Therefore deivide by two if you need it in 16bit int 
	unsigned int sampleLength;
	unsigned int loopStart;
	unsigned int loopLength;
	bool isLoopable;
	gmSoundRegion() 
		: rootNote(0u), lowest(0u), highest(0u), startByte(0u), sampleLength(0u), loopStart(0u), loopLength(0u), isLoopable(false) {};
};


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
	HANDLE* h;

	gmSound** interpolatedSounds;

	//works
	gmSoundRegion findTargetRegion(unsigned int rootNote) const;
public:
	gmInstrument(unsigned int numberOfRegions, gmSoundRegion* regions, HANDLE* handle);
	~gmInstrument();
	bool isLoopable() const;
	void getNote(unsigned int targetNote, Sample*& returnedBuffer, unsigned int& returnedBufferSize) const;
};

/**
 * Main gmsynthInstrument. Contains array of gmInstruments, where the soudns are stored for each note and sounds.
 */
class gmsynthInstrument : public InstrumentBase
{
public:
	gmsynthInstrument();
	~gmsynthInstrument();
	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	gmInstrument** midiinstruments;
	HANDLE h;
	void loadToHandle();
	void destroyHandle();
};

/**
 * Contains a note of a gmInstrument as a Sample array.
 * The constructor performs interpolation of the sample found based
 * on the gmSoundRegion handed over during construction.
 */
class gmSound
{
public:
	gmSound(gmSoundRegion& region, HANDLE* h, unsigned int targetNote);
	~gmSound();

	Sample* interpolatedBuffer;
	unsigned int interpolatedBufferSize;
	bool isLoopable;
	unsigned int loopStart;
	unsigned int loopStop;
	Sample* rootBuffer;
	gmSoundRegion mRegion;
private:

};