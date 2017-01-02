#pragma once

#include "MidiBase.h"


class pitchshiftMidiEffect : public MidiBase
{
public:
	pitchshiftMidiEffect();
	virtual void processMidiEvents(MidiEvent*& events, unsigned int numberOfMidiEvents) override;
};