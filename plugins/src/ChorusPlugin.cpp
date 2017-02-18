#include "ChorusPlugin.h"

#include "ChorusEffect.h"

ChorusPlugin::ChorusPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new ChorusEffect)
{
	setUniqueID('bhCh');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new ChorusPlugin(audioMaster);
}