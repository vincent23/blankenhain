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


MidiPluginBase::MidiPluginBase(audioMasterCallback const& audioMaster, MidiBase* instrument_)
	: PluginBase(audioMaster, instrument_), effect(*instrument_), midiEventPosition(0u)
{
	isSynth();
	// reserve space for 2048 midi events per vst process block
	midiEvents.reserve(2048);
}

VstInt32 MidiPluginBase::getNumMidiInputChannels()
{
	return 1;
}

VstInt32 MidiPluginBase::getNumMidiOutputChannels()
{
	return 1;
}

VstInt32 InstrumentPluginBase::getNumMidiInputChannels()
{
	return 1;
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

VstInt32 MidiPluginBase::processEvents(VstEvents* events)
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
		switch (static_cast<unsigned char>(midiEvent.midiData[0])) {
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

VstInt32 MidiPluginBase::canDo(char* text)
{
	if (!strcmp(text, "receiveVstEvents"))
		return 1;
	if (!strcmp(text, "receiveVstMidiEvent"))
		return 1;
	if (!strcmp(text, "sendVstEvents"))
		return 1;
	if (!strcmp(text, "sendVstMidiEvent"))
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

void MidiPluginBase::onAfterProcess()
{
	this->sendVstEventsToHost(& this->getMidiEventsAsVstEvents());
}

VstEvents MidiPluginBase::getMidiEventsAsVstEvents() const
{
	// some hacky shit to allocate VstEvents via
	// https://www.kvraudio.com/forum/viewtopic.php?t=214049

#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (constants::maxMidiEventsPerVSTEventBlock < midiEvents.size())
		throw std::runtime_error("Number of midiEvents in midiEvents vector bigger than constants::maxMidiEventsPerVSTEventBlock!\n");
#endif
	VSTEventBlock<constants::maxMidiEventsPerVSTEventBlock> block;

	block.numEvents = midiEvents.size();
	
	for (int i = 0; i < midiEvents.size(); i++) 
	{
		VstMidiEvent thisEvent;
		thisEvent.type = kVstMidiType;
		thisEvent.midiData[1] = midiEvents[i].key;
		thisEvent.midiData[2] = midiEvents[i].velocity;
		if (midiEvents[i].velocity == 0u)
			thisEvent.midiData[0] = static_cast<char>(0b10000000);
		else
			thisEvent.midiData[0] = static_cast<char>(0b10010000);
		block.events[i] = reinterpret_cast<VstEvent*>(new VstMidiEvent(thisEvent));
	}
	VstEvents* send = (VstEvents *)&block;

	return *send;
}

void MidiPluginBase::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames_)
{
	this->pluginParameters->updateParameters();
	
	onBeforeBlock(sampleFrames_);

	// Just copy audio, if there is any
	unsigned int sampleFrames = static_cast<unsigned int>(sampleFrames_);
	for (unsigned int i = 0; i < sampleFrames; i++) 
	{
		outputs[0][i] = inputs[0][i];
		outputs[1][i] = inputs[1][i];
	}
	MidiEvent* a = &midiEvents[0];

	effect.processMidiEvents(a, midiEvents.size());

	onAfterProcess();
}
