#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
#include "SoundFonts.h"

struct VoiceState;

class wavetableInstrument : public InstrumentBase
{
public:
	wavetableInstrument();
	~wavetableInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	WaveTableOscillator tri_osc;
	WaveTableOscillator sq_osc;
	WaveTableOscillator sawt_osc;
	WaveTableOscillator noise_osc1;
	WaveTableOscillator noise_osc2;
};