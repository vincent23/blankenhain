#pragma once

#include "InstrumentBase.h"
#include "Oscillators.h"
#include "SoundFonts.h"

struct VoiceState;

class basedrum1Instrument : public InstrumentBase
{
public:
	basedrum1Instrument();
	~basedrum1Instrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
private:
	//
	float drm1_lowpassfreq;
	float drm1_lowpassq;
	float drm1_pulselength;

	//

	I_SoundGenerator* currentGenerator;
	BassDrumSound2 drm2;
	BassDrumSound1 drm1;
};