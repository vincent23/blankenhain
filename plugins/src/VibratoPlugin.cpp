#include "VibratoPlugin.h"

#include "VibratoEffect.h"

VibratoPlugin::VibratoPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new VibratoEffect)
{
	setUniqueID('bhVb');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new VibratoPlugin(audioMaster);
}