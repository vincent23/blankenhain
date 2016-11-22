#include "VolumePlugin.h"

VolumePlugin::VolumePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new VolumeEffect)
{
	setUniqueID('bhVo');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new VolumePlugin(audioMaster);
}