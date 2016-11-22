#pragma once

#include "PluginBase.h"
#include "MidiEvent.h"

#include <vector>

class InstrumentBase;

class InstrumentPluginBase : public PluginBase
{
public:
	InstrumentPluginBase(audioMasterCallback const& audioMaster, InstrumentBase* instrument);

	VstInt32 getNumMidiInputChannels() override;
	VstInt32 processEvents(VstEvents* events) override;

private:
	void onBeforeBlock(unsigned int blockOffset) override;

	std::vector<MidiEvent> midiEvents;
	size_t midiEventPosition = 0;
	InstrumentBase& instrument;
};