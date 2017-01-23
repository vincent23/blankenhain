#include "InstrumentDevice.h"

#include "InstrumentBase.h"
#include "SongInfo.h"
#include "MidiTrack.h"

InstrumentDevice::InstrumentDevice(unsigned int blockSize, InstrumentBase& instrument, ParameterTrack* parameterValues, unsigned int inputTrackIndex_)
	: EffectDevice(blockSize, instrument, parameterValues)
	, inputTrackIndex(inputTrackIndex_)
{}

Sample* InstrumentDevice::process(SongInfo& songInfo, const Sample* input, unsigned int numberOfSamples, unsigned int globalSamplePosition)
{
	unsigned int velocity;
	unsigned int key;
	MidiTrack& midiTrack = songInfo.midiTracks[inputTrackIndex];
	while (midiTrack.getNextNote(globalSamplePosition, numberOfSamples, key, velocity)) {
		bool isNoteOn = velocity != 0;
		getInstrument().handleNoteEvent(isNoteOn, key, velocity);
	}
	EffectDevice::process(songInfo, input, numberOfSamples, globalSamplePosition);
	return outputBuffer;
}

InstrumentBase& InstrumentDevice::getInstrument()
{
	return static_cast<InstrumentBase&>(effect);
}