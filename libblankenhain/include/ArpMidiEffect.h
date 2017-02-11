#pragma once

#include "MidiBase.h"

class ArpMidiEffect : public MidiBase
{
private:
	bool whichNotesAreOn[128u];
	int oldShift;
public:
	ArpMidiEffect();
	virtual void processMidiEvents(MidiEvent* events, unsigned int& numberOfMidiEvents, unsigned int maxNumEvents, unsigned int numberOfSamples) override;
};