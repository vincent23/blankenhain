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

#ifndef _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH
#define _LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH 9
#endif

class FmInstrument : public InstrumentBase
{
private:
	// oscillators
	PolyBLEPOscillator osc[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH];

	// modulation information for each osc
	FmModulation mod[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH];

	I_SoundGenerator* currentSound;

	float lastOscValues[_LIBBLANKENHAIN_NUM_OSCS_FM_SYNTH];
	float lastCarrierValue;

	float freqPrev;
	unsigned int notePrev;
	unsigned int timeNoteOff;
public:
	FmInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
};