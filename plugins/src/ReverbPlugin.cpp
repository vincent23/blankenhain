#include "ReverbPlugin.h"

#include "ReverbEffect.h"

ReverbPlugin::ReverbPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new ReverbEffect)
{
	setUniqueID('bhfv');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new ReverbPlugin(audioMaster);
}