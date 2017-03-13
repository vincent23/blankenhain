#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
class VoiceState;

/**
 * Class that stores information on modulation for one target osc.
 */
class FmModulation
{
public:
	FmModulation()
	{
		fm = pm = am = false;
	}
	bool fm;
	float fmAmount;
	float fmVal;

	bool  pm;
	float pmAmount;
	float pmVal;

	bool  am;
	float amAmount;
	float amVal;
};


class FmInstrument : public InstrumentBase
{
private:
	static const unsigned int numOsc = 9u;

	// oscillators
	PolyBLEPOscillator osc[numOsc];

	// modulation information for each osc
	FmModulation mod[numOsc];

	I_SoundGenerator* currentSound;

	float lastOscValues[numOsc];
	float lastCarrierValue;

	float freqPrev;
	unsigned int notePrev;
	unsigned int timeNoteOff;
public:
	FmInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
};