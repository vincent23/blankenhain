#pragma once

// Changes in velocity of one note are not supported right now!
// event with velcocity == 0u is noteOffEvent
// event with velocity > 0u is noteOn Event
class MidiEvent
{
public:
	MidiEvent() = default;
	MidiEvent(unsigned int sampleOffset, unsigned int key, unsigned int velocity);
	bool isNoteOn() const;

	unsigned int sampleOffset;
	unsigned int key;
	unsigned int velocity;
};