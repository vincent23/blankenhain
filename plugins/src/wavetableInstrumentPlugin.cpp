#include "wavetableInstrumentPlugin.h"
#include "wavetableInstrument.h"

wavetableInstrumentPlugin::wavetableInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new wavetableInstrument)
{
	setUniqueID('bhWT');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new wavetableInstrumentPlugin(audioMaster);
}