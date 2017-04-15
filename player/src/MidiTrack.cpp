#include "MidiTrack.h"

MidiTrack::MidiTrack(unsigned int numberOfEvents_, unsigned int* samplePositions_, unsigned int* keys_, unsigned int* velocities_)
	: numberOfEvents(numberOfEvents_)
	, samplePositions(samplePositions_)
	, keys(keys_)
	, velocities(velocities_)
{
	unsigned int last = samplePositions[0];
	for (unsigned int i = 1; i < numberOfEvents; i++)
	{
		samplePositions[i] += last;
		last = samplePositions[i];
	}
}

bool MidiTrack::getNextNote(unsigned int samplePosition, unsigned int nextNoteIndex, unsigned int& keyOut, unsigned int& velocityOut)
{
	if (nextNoteIndex >= numberOfEvents || samplePosition < samplePositions[nextNoteIndex])
	{
		return false;
	}
	else
	{
		keyOut = keys[nextNoteIndex];
		velocityOut = velocities[nextNoteIndex];
		return true;
	}
}