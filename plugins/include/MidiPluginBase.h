#pragma once

#include "PluginBase.h"
#include "MidiEvent.h"
#include "MidiBase.h"
#include "PluginParameterBundle.h"
#include <vector>
#include "Options.h"
#include "Constants.h"

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
 * Be careful: As MIDI is only handed through when processReplacing is called,
 * take care that the audioBuffer is small enough, =< 1024 Samples should be fine.
 * 
 * via: https://www.kvraudio.com/forum/viewtopic.php?t=298368 
 */
class MidiPluginBase : public PluginBase
{
public:
	MidiPluginBase(audioMasterCallback const& audioMaster, MidiBase* effect);
	~MidiPluginBase();

	VstInt32 getNumMidiInputChannels() override;
	VstInt32 getNumMidiOutputChannels() override;

	/** 
	 * Is called by the HOST when Midi events are incoming
	 */
	VstInt32 processEvents(VstEvents* events) override;

	// Opcodes via http://ygrabit.steinberg.de/~ygrabit/public_html/vstgui/vstsupport5.html
	VstInt32 canDo(char* text) override;
	
	// Maybe its smart to provide a more lean processReplacing function for midi effects
	void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames_) final;

private:
	/**
	 * In here the MIDI is dispatched to the host DAW.
	 */
	void onAfterProcess() override;

	/**
	 * Converts internal vector of midiEvents to VstEvent*
	 * via internal helper members (see below)
	 * This is quite hacky...
	 */
	VstEvents* getMidiEventsAsVstEvents();
	VSTEventBlock eventBlock;
	VstMidiEvent* vstMidiEventInternalBuffer[constants::maxMidiEventsPerVSTEventBlock];

	std::vector<MidiEvent> midiEvents;
	MidiBase& effect;

};

void initializeAllEventsAsNull(VSTEventBlock& in);


