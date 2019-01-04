#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
#include "SoundFonts.h"

class VoiceState;

class PolysynthInstrument : public InstrumentBase
{
public:
	PolysynthInstrument();
	~PolysynthInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	PolyBLEPOscillator osc;
};