#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
#include "SoundFonts.h"

struct VoiceState;

class polyblepInstrument : public InstrumentBase
{
public:
	polyblepInstrument();
	~polyblepInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	PolyBLEPOscillator osc;
};