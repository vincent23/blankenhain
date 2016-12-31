#pragma once

#include "PluginBase.h"
#include "MidiEvent.h"

#include <vector>
#include "warnings.h"

class InstrumentBase;

class InstrumentPluginBase : public PluginBase
{
public:
	InstrumentPluginBase(audioMasterCallback const& audioMaster, InstrumentBase* instrument);

	VstInt32 getNumMidiInputChannels() override;
	VstInt32 getNumMidiOutputChannels() override;
	VstInt32 processEvents(VstEvents* events) override;
	VstInt32 canDo(char* text) override;
	VstPlugCategory getPlugCategory() final
	{
		return VstPlugCategory::kPlugCategSynth;
	}

private:
	void onBeforeBlock(unsigned int blockOffset) override;

	std::vector<MidiEvent> midiEvents;
	size_t midiEventPosition = 0;
	InstrumentBase& instrument;
};