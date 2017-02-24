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
	void process(Sample* buffer, size_t numberOfSamples, size_t currentTime) final;
	bool startNote(unsigned int key, unsigned int duration);
	void skipSamples(unsigned int samples);
	bool advanceToNextStopNote(unsigned int maxSamples, unsigned int& outSamplesAdvanced, unsigned int& outKey);

private:
	unsigned int getPosition(unsigned int index) const;

	unsigned int endPositions[constants::maxMidiNotesInFlight];
	unsigned int keys[constants::maxMidiNotesInFlight];
	unsigned int numberOfNotesInFlight = 0;
	unsigned int inFlightStartPosition = 0;
	unsigned int currentSamplePosition = 0;
};
