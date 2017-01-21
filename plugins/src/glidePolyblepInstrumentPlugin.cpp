#include "glidePolyblepInstrumentPlugin.h"
#include "glidePolyblepInstrument.h"
#include "glidePolyblepInstrumentPluginEditor.h"

glidePolyblepInstrumentPlugin::glidePolyblepInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new glidePolyblepInstrument)
{
	setUniqueID('bhgP');
	setEditor(new glidePolyblepInstrumentPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new glidePolyblepInstrumentPlugin(audioMaster);
}