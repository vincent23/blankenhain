#include "glidePolyblepInstrumentPlugin.h"
#include "glidePolyblepInstrument.h"

glidePolyblepInstrumentPlugin::glidePolyblepInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new glidePolyblepInstrument)
{
	setUniqueID('bhgP');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new glidePolyblepInstrumentPlugin(audioMaster);
}