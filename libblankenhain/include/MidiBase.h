#pragma once

#include "EffectBase.h"
#include "Constants.h"
#include "AuxFunc.h"
#include "MidiEvent.h"

class MidiBase : public EffectBase
{
public:
	MidiBase(unsigned int numberOfParameters) : EffectBase(numberOfParameters) {};
	virtual void processMidiEvents(MidiEvent*& events, unsigned int numberOfMidiEvents) = 0;
protected:
	virtual void process(Sample* buffer, size_t numberOfSamples) final 
	{
		this->nextSample(numberOfSamples);
	};
};
