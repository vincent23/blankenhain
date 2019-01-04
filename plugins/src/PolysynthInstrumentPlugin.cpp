#include "PolysynthInstrumentPlugin.h"
#include "PolysynthInstrument.h"
#include "PolysynthInstrumentPluginEditor.h"

PolysynthInstrumentPlugin::PolysynthInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new PolysynthInstrument)
{
	setUniqueID('bhPB');
	setEditor(new PolysynthInstrumentPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new PolysynthInstrumentPlugin(audioMaster);
}