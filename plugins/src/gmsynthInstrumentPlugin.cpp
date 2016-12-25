#include "gmsynthInstrumentPlugin.h"
#include "gmsynthInstrument.h"

gmsynthInstrumentPlugin::gmsynthInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new gmsynthInstrument)
{
	setUniqueID('bhGM');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new gmsynthInstrumentPlugin(audioMaster);
}