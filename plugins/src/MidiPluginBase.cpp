#include "MidiPluginBase.h"

#include "MidiBase.h"
#include "Constants.h"


MidiPluginBase::MidiPluginBase(audioMasterCallback const& audioMaster, MidiBase* instrument_)
	: PluginBase(audioMaster, instrument_), effect(*instrument_), eventBlock()
{

	for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
		vstMidiEventInternalBuffer[i] = new VstMidiEvent;


	isSynth();
	// reserve space for 2048 midi events per vst process block
	midiEvents.reserve(constants::maxMidiEventsPerVSTEventBlock);
	initializeAllEventsAsNull(this->eventBlock);
}

MidiPluginBase::~MidiPluginBase()
{
	for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
		delete vstMidiEventInternalBuffer[i];
}

VstInt32 MidiPluginBase::getNumMidiInputChannels()
{
	return 1;
}

VstInt32 MidiPluginBase::getNumMidiOutputChannels()
{
	return 1;
}

VstInt32 MidiPluginBase::processEvents(VstEvents* events)
{
	if (midiEvents.size() != 0u)
		this->midiEvents.clear();
	// assume events arrive in the correct order
	for (int i = 0; i < events->numEvents; i++) 
	{
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

void MidiPluginBase::onAfterProcess()
{
	if (this->midiEvents.size() > 0u)
	{
		VstEvents* vstevents = this->getMidiEventsAsVstEvents();
		while(!this->sendVstEventsToHost(vstevents));
		this->midiEvents.clear();
	}
}


VstEvents* MidiPluginBase::getMidiEventsAsVstEvents()
{
	// some hacky shit to allocate VstEvents via
	// https://www.kvraudio.com/forum/viewtopic.php?t=214049

#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
	if (constants::maxMidiEventsPerVSTEventBlock < midiEvents.size())
		throw std::runtime_error("Number of midiEvents in midiEvents vector bigger than constants::maxMidiEventsPerVSTEventBlock!\n");
#endif
	// Delete old events
	for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
	{
		*(vstMidiEventInternalBuffer[i]) = VstMidiEvent();
	}

	this->eventBlock.numEvents = midiEvents.size();
	
	for (unsigned int i = 0; i < midiEvents.size(); i++) 
	{
		VstMidiEvent& thisEvent = *(vstMidiEventInternalBuffer[i]);
		thisEvent.type = kVstMidiType;
		thisEvent.midiData[1] = midiEvents[i].key;
		thisEvent.midiData[2] = midiEvents[i].velocity;
		if (midiEvents[i].velocity == 0u)
			thisEvent.midiData[0] = static_cast<unsigned char>(0b10000000);
		else
			thisEvent.midiData[0] = static_cast<unsigned char>(0b10010000);
		eventBlock.events[i] = reinterpret_cast<VstEvent*>(vstMidiEventInternalBuffer[i]);
	}
	return (VstEvents *)&eventBlock;
}

void MidiPluginBase::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames_)
{
	onBeforeBlock(sampleFrames_);

	// Call this to get parameter values straight (interpolation). Yes this is necessary.
	// Exept for parameter stuff nothing is done here.
	effect.processBlock(nullptr, sampleFrames_);
	this->pluginParameters->updateParameters();

	// Just copy audio, if there is any
	// TODO: eliminate this entirely maybe?
	unsigned int sampleFrames = static_cast<unsigned int>(sampleFrames_);
	for (unsigned int i = 0; i < sampleFrames; i++) 
	{
		outputs[0][i] = inputs[0][i];
		outputs[1][i] = inputs[1][i];
	}
	
	unsigned int midiEventNum = midiEvents.size();
	midiEvents.resize(256, MidiEvent(0u,0u,0u));
	MidiEvent* a = &midiEvents[0];

	// Here, the magic happens
	effect.processMidiEvents(a, midiEventNum, midiEvents.size(), sampleFrames_);
	midiEvents.resize(midiEventNum, MidiEvent(0u, 0u, 0u));

	// Midi is dispatched in here
	onAfterProcess();

	incrementTempoDataPosition(sampleFrames_);
}

void initializeAllEventsAsNull(VSTEventBlock& in)
{
	in.numEvents = 0u;
	for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
	{
		in.events[i] = nullptr;
	}
}
