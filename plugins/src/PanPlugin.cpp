#include "PanPlugin.h"

#include "PanEffect.h"

PanPlugin::PanPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new PanEffect)
{
	setUniqueID('bhPa');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new PanPlugin(audioMaster);
}