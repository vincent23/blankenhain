#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
class VoiceState;


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
	PolyBLEPOscillator osc[numOsc];
	FmModulation mod[numOsc];
	float lastValue[numOsc];
	I_SoundGenerator* currentSound;
public:
	FmInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
};