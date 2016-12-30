#include "basedrum1InstrumentPlugin.h"
#include "basedrum1Instrument.h"

basedrum1InstrumentPlugin::basedrum1InstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new basedrum1Instrument)
{
	setUniqueID('bhB1');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new basedrum1InstrumentPlugin(audioMaster);
}