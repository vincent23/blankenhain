// Change header here
#include "mverbPlugin.h"

#include "mverbEffect.h"

mverbPlugin::mverbPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new mverbEffect)
{
	setUniqueID('bhmv');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new mverbPlugin(audioMaster);
}