#pragma once

#include "MidiPluginBase.h"

class pitchshiftMidiPlugin : public MidiPluginBase
{
public:
	pitchshiftMidiPlugin(audioMasterCallback audioMaster);
};