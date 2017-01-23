#pragma once

class MidiTrack
{
public:
	MidiTrack(unsigned int numberOfEvents, unsigned int* samplePositions, unsigned int* keys, unsigned int* velocities);
	bool getNextNote(unsigned int samplePosition, unsigned int nextNoteIndex, unsigned int& keyOut, unsigned int& velocityOut);

private:
	unsigned int numberOfEvents;
	unsigned int* samplePositions;
	unsigned int* keys;
	unsigned int* velocities;
};