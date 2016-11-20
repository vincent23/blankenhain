#include "VolumePlugin.h"

VolumePlugin::VolumePlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new VolumeEffect)
{ }

void VolumePlugin::open()
{
	setUniqueID(189);
	isSynth(false);
}

void VolumePlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new VolumePlugin(audioMaster);
}