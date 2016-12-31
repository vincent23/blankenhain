#pragma once

#include "PluginBase.h"
#include "MidiEvent.h"
#include "InstrumentBase.h"
#include "PluginParameterBundle.h"
#include <vector>
#include "warnings.h"

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
	VstEvents getMidiEventsAsVstEvents() const;

	std::vector<MidiEvent> midiEvents;
	size_t midiEventPosition;
	MidiBase& effect;
};

// via https://www.kvraudio.com/forum/viewtopic.php?t=214049
template <int N>
struct VSTEventBlock
{
	int numEvents;
	int reserved;
	VstEvent* events[N];
};