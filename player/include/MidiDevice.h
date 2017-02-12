#pragma once

#include "EffectDevice.h"

class MidiBase;

class MidiDevice : public EffectDevice
{
public:
	MidiDevice(MidiBase& midiEffect, ParameterTrack* parameterValues, unsigned int inputTrackIndex, unsigned int outputTrackIndex);

	void processMidi(SongInfo& songInfo);

private:
	MidiBase& getMidiEffect();
	unsigned int inputTrackIndex;
	unsigned int outputTrackIndex;
	unsigned int nextNoteIndex = 0;
};