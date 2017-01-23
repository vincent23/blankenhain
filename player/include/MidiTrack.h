#pragma once

class MidiTrack
{
public:
	MidiTrack(unsigned int numberOfEvents, unsigned int* sampleOffsets, unsigned int* keys, unsigned int* velocities);
	bool getNextNote(unsigned int blockOffset, unsigned int numberOfSamples, unsigned int& keyOut, unsigned int& velocityOut);

private:
	unsigned int numberOfEvents;
	unsigned int* sampleOffsets;
	unsigned int* keys;
	unsigned int* velocities;
};