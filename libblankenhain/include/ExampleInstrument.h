#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
class VoiceState;

class ExampleInstrument : public InstrumentBase
{
private:
	NaiveOscillator osc;
	NoiseOscillator nosc;
	I_SoundGenerator* currentSound;
public:
	ExampleInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
};