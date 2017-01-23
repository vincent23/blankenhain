#include "MidiTrack.h"

MidiTrack::MidiTrack(unsigned int numberOfEvents_, unsigned int* sampleOffsets_, unsigned int* keys_, unsigned int* velocities_)
	: numberOfEvents(numberOfEvents_)
	, sampleOffsets(sampleOffsets_)
	, keys(keys_)
	, velocities(velocities_)
{}

bool MidiTrack::getNextNote(unsigned int blockOffset, unsigned int& keyOut, unsigned int& velocityOut)
{
	keyOut = 0;
	velocityOut = 0;
	return false;
}