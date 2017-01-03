#include "FmInstrumentPlugin.h"
#include "FmInstrument.h"
#include "FmInstrumentPluginEditor.h"

FmInstrumentPlugin::FmInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new FmInstrument)
{
	setUniqueID('bhFm');
	setEditor(new FmInstrumentPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new FmInstrumentPlugin(audioMaster);
}