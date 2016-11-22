#include "DelayPlugin.h"

#include "DelayEffect.h"

DelayPlugin::DelayPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new DelayEffect)
{
	setUniqueID('bhDe');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new DelayPlugin(audioMaster);
}