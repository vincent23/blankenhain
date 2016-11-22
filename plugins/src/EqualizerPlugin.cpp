#include "EqualizerPlugin.h"

#include "EqualizerEffect.h"

EqualizerPlugin::EqualizerPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new EqualizerEffect)
{
	setUniqueID('bhEq');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new EqualizerPlugin(audioMaster);
}