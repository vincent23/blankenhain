#include "CompressorPlugin.h"

#include "CompressorEffect.h"

CompressorPlugin::CompressorPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new CompressorEffect)
{ }

void CompressorPlugin::open()
{
	setUniqueID(1993);
	isSynth(false);
}

void CompressorPlugin::close()
{
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new CompressorPlugin(audioMaster);
}