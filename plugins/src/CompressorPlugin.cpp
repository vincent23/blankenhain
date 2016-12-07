#include "CompressorPlugin.h"

#include "CompressorEffect.h"
#include "CompressorPluginEditor.h"

CompressorPlugin::CompressorPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new CompressorEffect)
{
	setUniqueID('bhCo');
	setEditor(new CompressorPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new CompressorPlugin(audioMaster);
}