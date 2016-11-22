#include "ExampleInstrumentPlugin.h"
#include "ExampleInstrument.h"

ExampleInstrumentPlugin::ExampleInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new ExampleInstrument)
{
	setUniqueID('bhEI');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new ExampleInstrumentPlugin(audioMaster);
}