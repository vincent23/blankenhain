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

protected:
	void onBeforeBlock(unsigned int blockOffset) override;

private:
	unsigned int loadedInstrument = 0;
};