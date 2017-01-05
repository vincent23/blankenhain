#pragma once

#include "InstrumentBase.h"
#include "gmInstrument.h"


using HANDLE = void*;

class VoiceState;
class gmSound;

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
