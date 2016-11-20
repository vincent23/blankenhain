#include "DistortionPlugin.h"

#include "DistortionEffect.h"

DistortionPlugin::DistortionPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new DistortionEffect)
{ }

void DistortionPlugin::open()
{
	setUniqueID(189);
	isSynth(false);
}

void DistortionPlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new DistortionPlugin(audioMaster);
}