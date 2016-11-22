#include "CompressorPlugin.h"

#include "CompressorEffect.h"

CompressorPlugin::CompressorPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new CompressorEffect)
{
	setUniqueID('bhCo');
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new CompressorPlugin(audioMaster);
}