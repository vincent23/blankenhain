#pragma once

#include "MidiBase.h"


class pitchshiftMidiEffect : public MidiBase
{
private:
	bool whichNotesAreOn[128u];
	int oldShift;
public:
	pitchshiftMidiEffect();
	virtual void processMidiEvents(MidiEvent*& events, unsigned int& numberOfMidiEvents, unsigned int maxNumEvents) override;
};