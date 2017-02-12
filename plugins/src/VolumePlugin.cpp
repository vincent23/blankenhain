#include "VolumePlugin.h"

VolumePlugin::VolumePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new VolumeEffect)
{
	setUniqueID('bhVo');
	setEditor(new VolumePluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new VolumePlugin(audioMaster);
}