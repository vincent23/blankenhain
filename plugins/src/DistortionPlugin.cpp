#include "DistortionPlugin.h"

#include "DistortionEffect.h"

DistortionPlugin::DistortionPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new DistortionEffect)
{
	setUniqueID('bhDi');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new DistortionPlugin(audioMaster);
}