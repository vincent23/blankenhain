#pragma once

#include "PluginBase.h"
#include "MidiEvent.h"
#include "InstrumentBase.h"
#include "PluginParameterBundle.h"
#include <vector>
#include "warnings.h"
#include "Constants.h"

class InstrumentBase;

class InstrumentPluginBase : public PluginBase
{
public:
	InstrumentPluginBase(audioMasterCallback const& audioMaster, InstrumentBase* instrument);

	VstInt32 getNumMidiInputChannels() override;
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

// via https://www.kvraudio.com/forum/viewtopic.php?t=214049
struct VSTEventBlock
{
	int numEvents;
	int reserved;
	VstEvent* events[constants::maxMidiEventsPerVSTEventBlock];
};


/**
 * We can't generate MIDI Plugins to be placed before VSTi Plugins in ableton
 * as the itnernal ableton plugins do. So we have to handle rerouting.
 *
 * via: https://www.kvraudio.com/forum/viewtopic.php?t=298368 
 */
class MidiPluginBase : public PluginBase
{
public:
	MidiPluginBase(audioMasterCallback const& audioMaster, MidiBase* effect);

	VstInt32 getNumMidiInputChannels() override;
	VstInt32 getNumMidiOutputChannels() override;
	VstInt32 processEvents(VstEvents* events) override;
	// Opcodes via http://ygrabit.steinberg.de/~ygrabit/public_html/vstgui/vstsupport5.html
	VstInt32 canDo(char* text) override;
	
	// Maybe its smart to provide a more lean processReplacing function for midi effects
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames_) final;

private:
	void onAfterProcess() override;

	VstEvents* getMidiEventsAsVstEvents();
	VstEvents* vstevents;
	VSTEventBlock eventBlock;

	std::vector<MidiEvent> midiEvents;
	size_t midiEventPosition;
	MidiBase& effect;
	void initializeAllEventsAsNull(VSTEventBlock& in)
	{
		in.numEvents = 0u;
		for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
		{
			in.events[i] = nullptr;
		}
	}
	void deleteEventsInVSTEventsBlock(VSTEventBlock& in)
	{
		for (unsigned int i = 0u; i < constants::maxMidiEventsPerVSTEventBlock; i++)
		{
			if (in.events[i] != nullptr)
				delete reinterpret_cast<VstMidiEvent*>(in.events[i]);
		}
	};
};




