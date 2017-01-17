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
	VstPlugCategory getPlugCategory() final;
	void suspend() override;
	void close() override;

protected:
  InstrumentBase& instrument;
	void onBeforeBlock(unsigned int blockOffset) override;
private:
	std::vector<MidiEvent> midiEvents;
	size_t midiEventPosition = 0;

};