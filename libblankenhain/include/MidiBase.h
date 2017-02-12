#pragma once

#include "EffectBase.h"
#include "Constants.h"
#include "AuxFunc.h"
#include "MidiEvent.h"

class MidiBase : public EffectBase
{
public:
	MidiBase(unsigned int numberOfParameters, bool usesTempoDate = false) : EffectBase(numberOfParameters, usesTempoDate) {};
	virtual void processMidiEvents(MidiEvent* events, unsigned int& numberOfMidiEvents, unsigned int maxNumEvents, unsigned int numberOfSamples) = 0;
protected:
	virtual void process(Sample* buffer, size_t numberOfSamples) final
	{
		this->nextSample(numberOfSamples);
	};
};
