#include "polyblepInstrumentPlugin.h"
#include "polyblepInstrument.h"

polyblepInstrumentPlugin::polyblepInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new polyblepInstrument)
{
	setUniqueID('bhPB');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new polyblepInstrumentPlugin(audioMaster);
}