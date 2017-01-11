#pragma once

#include "InstrumentPluginBase.h"

class gmInstrument;

class gmsynthInstrumentPlugin : public InstrumentPluginBase
{
public:
	gmsynthInstrumentPlugin(audioMasterCallback audioMaster);
	~gmsynthInstrumentPlugin();

	void loadMidiInstrument(unsigned int index);

	gmInstrument** instruments;
};