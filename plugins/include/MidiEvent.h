#pragma once

struct MidiEvent
{
	MidiEvent(unsigned int sampleOffset, unsigned int key, unsigned int velocity);
	bool isNoteOn() const;

	unsigned int sampleOffset;
	unsigned int key;
	unsigned int velocity;
};