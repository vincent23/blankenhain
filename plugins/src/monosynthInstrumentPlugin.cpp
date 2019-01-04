#include "monosynthInstrumentPlugin.h"
#include "monosynthInstrument.h"
#include "monosynthInstrumentPluginEditor.h"

monosynthInstrumentPlugin::monosynthInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new monosynthInstrument)
{
	setUniqueID('bhgP');
	setEditor(new monosynthInstrumentPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new monosynthInstrumentPlugin(audioMaster);
}