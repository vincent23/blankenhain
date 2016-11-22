#pragma once

#include "EffectBase.h"
#include "Constants.h"

class VoiceState;

class InstrumentBase : public EffectBase
{
public:
	InstrumentBase(unsigned int numberOfParameters, unsigned int numberOfVoices);

	void handleNoteEvent(bool isNoteOn, unsigned int key, unsigned int velocity);
	void process(Sample* buffer, size_t numberOfSamples) final;

	virtual void processVoice(VoiceState& voice, unsigned int timeInSamples, Sample* buffer, size_t numberOfSamples) = 0;

private:
	Sample voiceBuffer[constants::blockSize];
	unsigned int numberOfVoices;
	VoiceState* voices;
	unsigned int timeInSamples = 0;
};