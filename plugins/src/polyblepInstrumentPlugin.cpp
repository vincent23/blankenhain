#include "polyblepInstrumentPlugin.h"
#include "polyblepInstrument.h"
#include "polyblepInstrumentPluginEditor.h"

polyblepInstrumentPlugin::polyblepInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new polyblepInstrument)
{
	setUniqueID('bhPB');
	//setEditor(new polyblepInstrumentPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new polyblepInstrumentPlugin(audioMaster);
}