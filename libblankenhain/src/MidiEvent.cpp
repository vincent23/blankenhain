#include "MidiEvent.h"

MidiEvent::MidiEvent(unsigned int sampleOffset_, unsigned int key_, unsigned int velocity_)
	: sampleOffset(sampleOffset_)
	, key(key_)
	, velocity(velocity_)
{ }

bool MidiEvent::isNoteOn() const {
	return velocity != 0;
}