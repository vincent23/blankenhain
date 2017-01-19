#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
#include "SoundFonts.h"

class VoiceState;

class glidePolyblepInstrument : public InstrumentBase
{
public:
	glidePolyblepInstrument();
	~glidePolyblepInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	PolyBLEPOscillator osc;
	float freqPrev;
	unsigned int notePrev;
	unsigned int timeNoteOff;
};