#include "EqualizerPlugin.h"

#include "EqualizerEffect.h"

EqualizerPlugin::EqualizerPlugin(audioMasterCallback audioMaster)
	: PluginBase(audioMaster, new EqualizerEffect)
{ }

void EqualizerPlugin::open()
{
	setUniqueID(189);
	isSynth(false);
}

void EqualizerPlugin::close()
{

}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new EqualizerPlugin(audioMaster);
}