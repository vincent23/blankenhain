#include "MidiDevice.h"

#include "MidiBase.h"
#include "SongInfo.h"
#include "MidiTrack.h"

MidiDevice::MidiDevice(MidiBase& midiEffect, ParameterTrack* parameterValues, unsigned int inputTrackIndex_, unsigned int outputTrackIndex_)
	: EffectDevice(midiEffect, parameterValues)
	, inputTrackIndex(inputTrackIndex_)
	, outputTrackIndex(outputTrackIndex_)
{}

void MidiDevice::processMidi(SongInfo& songInfo)
{
	Sample dummyInput[constants::parameterInterpolationLength];
	const unsigned int eventBufferSize = 2048;
	MidiEvent midiEventBuffer[eventBufferSize];
	MidiTrack& midiInputTrack = *(songInfo.midiTracks[inputTrackIndex]);
	MidiTrack& midiOutputTrack = *(songInfo.midiTracks[outputTrackIndex]);
	unsigned int maxOutputEvents = midiOutputTrack.numberOfEvents;
	midiOutputTrack.numberOfEvents = 0;
	for (unsigned int globalSamplePosition = 0; globalSamplePosition < songInfo.lengthInSamples; globalSamplePosition += constants::parameterInterpolationLength) 
	{
		// call process just to set parameters and stuff
		EffectDevice::process(songInfo, dummyInput, globalSamplePosition);
		unsigned int numberOfEvents = 0;
		unsigned int velocity;
		unsigned int key;
		while (midiInputTrack.getNextNote(globalSamplePosition, nextNoteIndex, key, velocity)) 
		{
			midiEventBuffer[numberOfEvents++] = MidiEvent(globalSamplePosition, key, velocity);
			nextNoteIndex++;
		}
		getMidiEffect().processMidiEvents(midiEventBuffer, numberOfEvents, eventBufferSize, constants::parameterInterpolationLength);
		for (unsigned int eventIndex = 0; eventIndex < numberOfEvents; eventIndex++) 
		{
			unsigned int& outIndex = midiOutputTrack.numberOfEvents;
			midiOutputTrack.keys[outIndex] = midiEventBuffer[eventIndex].key;
			midiOutputTrack.velocities[outIndex] = midiEventBuffer[eventIndex].velocity;
			midiOutputTrack.samplePositions[outIndex] = midiEventBuffer[eventIndex].sampleOffset;
			outIndex++;
		}
	}
}

MidiBase& MidiDevice::getMidiEffect()
{
	return static_cast<MidiBase&>(effect);
}