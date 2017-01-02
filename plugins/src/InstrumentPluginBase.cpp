#include "InstrumentPluginBase.h"

#include "InstrumentBase.h"
#include "Constants.h"


InstrumentPluginBase::InstrumentPluginBase(audioMasterCallback const& audioMaster, InstrumentBase* instrument_)
	: PluginBase(audioMaster, instrument_)
	, instrument(*instrument_)
{
	isSynth();
	// reserve space for 2048 midi events per vst process block
	midiEvents.reserve(2048);
}

VstInt32 InstrumentPluginBase::getNumMidiInputChannels()
{
	return 1;
}

VstPlugCategory InstrumentPluginBase::getPlugCategory()
{
	return VstPlugCategory::kPlugCategSynth;
}

void InstrumentPluginBase::suspend()
{
	instrument.resetVoices();
}

void InstrumentPluginBase::close()
{
	instrument.resetVoices();
}

VstInt32 InstrumentPluginBase::processEvents(VstEvents* events)
{
	size_t remaining = midiEvents.size() - midiEventPosition;
	std::move(midiEvents.begin() + midiEventPosition, midiEvents.end(), midiEvents.begin());
	// because we do not have a default constructor we have to supply a dummy element.
	// since we always shrink the vector, this won't be used.
	midiEvents.resize(remaining, MidiEvent(0, 0, 0));
	for (unsigned int i = 0; i < remaining; i++) {
		midiEvents[i].sampleOffset = 0;
	}
	midiEventPosition = 0;

	// assume events arrive in the correct order
	for (int i = 0; i < events->numEvents; i++) {
		VstEvent* vstEvent = events->events[i];
		if (vstEvent->type != kVstMidiType) {
			// we only care about MIDI events (and ignore sysex ones)
			continue;
		}
		VstMidiEvent& midiEvent = *reinterpret_cast<VstMidiEvent*>(vstEvent);
		unsigned int key = midiEvent.midiData[1];
		unsigned int velocity = midiEvent.midiData[2];
		switch(static_cast<unsigned char>(midiEvent.midiData[0])) {
		case 0b10000000:
			// set velocity to 0 to indicate a note off event
			velocity = 0;
		case 0b10010000:
			// note on event
			// zero velocity is interpreted as note off in accordance to the midi standard
			midiEvents.emplace_back(midiEvent.deltaFrames, key, velocity);
		}
	}
	return 0;
}


VstInt32 InstrumentPluginBase::canDo(char* text)
{
	if (!strcmp(text, "receiveVstEvents"))
		return 1;
	if (!strcmp(text, "receiveVstMidiEvent"))
		return 1;
	return -1;
}


void InstrumentPluginBase::onBeforeBlock(unsigned int blockOffset)
{
	unsigned int halfBlockSize = constants::blockSize;
	for (unsigned int eventIndex = midiEventPosition; eventIndex < midiEvents.size(); eventIndex++) {
		const MidiEvent& midiEvent = midiEvents[eventIndex];
		// align midi notes to nearest block start (first half of block is aligned to that block, second half to the next one)
		if (blockOffset <= midiEvent.sampleOffset + halfBlockSize && midiEvent.sampleOffset < blockOffset + halfBlockSize) {
			instrument.handleNoteEvent(midiEvent.isNoteOn(), midiEvent.key, midiEvent.velocity);
			midiEventPosition++;
		}
		else {
			break;
		}
	}
}
