// Change header here
#include "WidthPlugin.h"

#include "WidthEffect.h"

WidthPlugin::WidthPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new WidthEffect)
{
	setUniqueID('bhWd');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new WidthPlugin(audioMaster);
}