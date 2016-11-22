#pragma once

#include "InstrumentBase.h"

struct VoiceState;

class ExampleInstrument : public InstrumentBase
{
public:
	ExampleInstrument();

	void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, unsigned int numberOfSamples);
};