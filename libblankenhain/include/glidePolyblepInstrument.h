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

	virtual void getModulation(float* modulationValues, size_t sampleOffset = 0) override;

private:
	PolyBLEPOscillator osc;
	NoiseOscillator noise;
	PolyBLEPOscillator lfo;
	BaseOscillator* currentOsc;

	float freqPrev;
	unsigned int notePrev;
	unsigned int timeNoteOff;
};