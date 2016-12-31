#pragma once

#include "InstrumentBase.h"


class pitchshiftMidiEffect : public MidiBase
{
private:

public:
	pitchshiftMidiEffect();
	virtual void processMidiEvents(MidiEvent*& events, unsigned int numberOfMidiEvents) override;
};