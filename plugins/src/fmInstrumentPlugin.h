#pragma once
#include "FmInstrument.h"
#include "InstrumentPluginBase.h"

class FmInstrumentPlugin : public InstrumentPluginBase
{
private:
public:
	FmInstrumentPlugin(audioMasterCallback audioMaster);
};