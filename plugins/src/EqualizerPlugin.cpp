#include "EqualizerPlugin.h"

#include "EqualizerEffect.h"
#include "EqualizerPluginEditor.h"

EqualizerPlugin::EqualizerPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new EqualizerEffect)
{
	setUniqueID('bhEq');
	setEditor(new EqualizerPluginEditor(this));
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new EqualizerPlugin(audioMaster);
}