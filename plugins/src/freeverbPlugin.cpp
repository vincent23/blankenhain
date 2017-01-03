// Change header here
#include "freeverbPlugin.h"

#include "freeverbEffect.h"

freeverbPlugin::freeverbPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new freeverbEffect)
{
	setUniqueID('bhfv');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new freeverbPlugin(audioMaster);
}